#include <iostream>
#include "RoboMotor.h"
#include "RoboPort.h"
#include <boost/thread/thread.hpp>

using namespace std;

namespace Nautilus{

	RoboMotor::RoboMotor(Nautilus::RoboPort *p, int pinNumber)
	{
		/*
			Initializes the Motor with a pointer to a RoboPort, and the motor's pinNumber.
			Sets all of the speeds to the initial 0.
		*/
		port = p;
		this->pinNumber = pinNumber;
		xSpeed = 0;
		ySpeed = 0;
		zSpeed = 0;
		yawSpeed = 0;
		pitchSpeed = 0;
		rollSpeed = 0;
		previousSpeed = 0;
		turnSpeed = 0;
	}
	double RoboMotor::getTotalSpeed()
	{
		/*
			Returns all of the individual speed variables added together.
		*/
		return (getXSpeed() + getYSpeed() + getZSpeed() + getYawSpeed() 
			+ getPitchSpeed() + getRollSpeed() + getTurnSpeed());
	}
	double RoboMotor::getXSpeed(){mutexXSpeed.lock(); double t = xSpeed; mutexXSpeed.unlock(); return t;}					//Returns the X speed.
	double RoboMotor::getYSpeed(){mutexYSpeed.lock(); double t = ySpeed; mutexYSpeed.unlock(); return t;}					//Returns the Y speed.
	double RoboMotor::getZSpeed(){mutexZSpeed.lock(); double t = zSpeed; mutexZSpeed.unlock(); return t;}					//Returns the Z speed.
	double RoboMotor::getTurnSpeed(){mutexTurnSpeed.lock(); double t = turnSpeed; mutexTurnSpeed.unlock(); return t;}		//Returns the Turn Speed.
	double RoboMotor::getYawSpeed(){mutexYawSpeed.lock(); double t = yawSpeed; mutexYawSpeed.unlock(); return t;}			//Returns the Yaw Speed.
	double RoboMotor::getPitchSpeed(){mutexPitchSpeed.lock(); double t = pitchSpeed; mutexPitchSpeed.unlock(); return t;}	//Returns the Pitch Speed.
	double RoboMotor::getRollSpeed(){mutexRollSpeed.lock(); double t = rollSpeed; mutexRollSpeed.unlock(); return t;}		//Returns the Roll Speed.
	double RoboMotor::getPreviousSpeed(){mutexPreviousSpeed.lock(); double t = previousSpeed; mutexPreviousSpeed.unlock(); return t;}	//Returns the previous totalSpeed recorded.
	int RoboMotor::getPinNumber(){return pinNumber;}	//Returns the motor's pinNumber.

	void RoboMotor::setXSpeed(double t)
	{
		/*
			Sets the motor's X Speed.
			X speed is for Strafing left or right.
		*/
		mutexXSpeed.lock();
		xSpeed = t;
		mutexXSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setYSpeed(double t)
	{
		/*
			Sets the motor's Y Speed.
			Y speed is for moving forward and reverse.
		*/
		mutexYSpeed.lock();
		ySpeed = t;
		mutexYSpeed.unlock();
		boost::thread thr(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setZSpeed(double t)
	{
		/*
			Sets the motor's Z Speed.
			Z speed is for moving up and down.
		*/
		mutexZSpeed.lock();
		zSpeed = t;
		mutexZSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setTurnSpeed(double t)
	{
		/*
			Sets the motor's Turn Speed.
			turnSpeed is used for turning left and right.
		*/
		mutexTurnSpeed.lock();
		turnSpeed = t;
		mutexTurnSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setYawSpeed(double t)
	{
		/*
			Sets the motor's Yaw Speed.
			Yaw speed is used for for balancing the submarines Yaw.
		*/
		mutexYawSpeed.lock();
		yawSpeed = t;
		mutexYawSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setPitchSpeed(double t)
	{
		/*
			Sets the motor's Pitch Speed.
			Pitch speed is used for for balancing the submarines Pitch.
		*/
		mutexPitchSpeed.lock();
		pitchSpeed = t;
		mutexPitchSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setRollSpeed(double t)
	{
		/*
			Sets the motor's Roll Speed.
			Roll speed is used for for balancing the submarines Roll.
		*/
		mutexRollSpeed.lock();
		rollSpeed = t;
		mutexRollSpeed.unlock();
		boost::thread thread(&RoboMotor::accelerateMotorSmoothly,this);
	}
	void RoboMotor::setPreviousSpeed(double t)
	{
		/*
			Returns the variable that held the previously logged speed.
			This is mainly used to determine how much acceleration we have to
			account for in the case that the previous speed is much different than our
			current speed.
		*/
		mutexPreviousSpeed.lock();
		previousSpeed = t;
		mutexPreviousSpeed.unlock();
	}
	void RoboMotor::accelerateMotorSmoothly()
	{
		/*
			Causes the motors to slowly accelerate rather than a huge jump between speeds.
			This function was inspired from the motors causing the entire electrical system to crash
			because it was pulling too much power all at one time.
		*/
		const int ACCELERATION_STEP = 20;		//The amount of value to increase per acceleration step
		const int ACCELERATION_DELAY = 50;		//How many milliseconds between each step
		const int MIDDLE_SPEED = 0;			//The middle speed between positive and negative speeds

		//Increments the speed that the motor needs to adjust,
		//this function will help reduce the motors from reving up too fast
		//mutexAccelerateMotor.lock();
		
		double prevSpeed = getPreviousSpeed();
		double thisSpeed = getTotalSpeed();
		//cout << "Pitch:" << pitchSpeed << "\tRoll:" << rollSpeed << "\tPrev:" << prevSpeed << "\tSpeed:" << thisSpeed << endl;
		
		//Do some calculations to slowly accelerate the motor depending on the constant
		int amountOfSteps = (int)abs(thisSpeed - prevSpeed) / ACCELERATION_STEP;
		int remainderSpeed = (int)abs(thisSpeed - prevSpeed) % ACCELERATION_STEP; //The amount left over
		int direction = 1; //This is used to determine if we need to adjust forward or backword

		if(thisSpeed < prevSpeed) direction = -1;
		//cout << "Prev: " << prevSpeed << ", ThisSpeed: " << thisSpeed << ", Steps: " << amountOfSteps << ", Remain: " << remainderSpeed << ", Dir: " << direction << endl;

		for(int i = 0; i < amountOfSteps; i++)
		{
			writeToPort(prevSpeed + (i * ACCELERATION_STEP * direction) + MIDDLE_SPEED);
			timer.wait(ACCELERATION_DELAY);
		}
		//if(remainderSpeed != 0)
			writeToPort(prevSpeed + (amountOfSteps * ACCELERATION_STEP * direction) + (remainderSpeed * direction) + MIDDLE_SPEED); //Add the leftovers
		setPreviousSpeed(thisSpeed);
		//mutexAccelerateMotor.unlock();
	}
	void RoboMotor::writeToPort(double speed)
	{
		/*
			This function writes a speed to the corresponding arduino pin number.
			The arduino must be setup to receive the speed based on the following protocol
			M:#,#
			Aka M:pinNumber,Speed

			Motor Scale:
				44 Full Reverse
				94 Mid
				144 Full Forward
		*/
		// Adjust the motor scale so that if we send a number between -100 and 
		// 100 it is readjusted to be between 44-144.
		double scaledSpeed = speed;
		scaledSpeed/=2;
		scaledSpeed+=94;

		std::ostringstream ost;
		ost << "M:" << pinNumber << "," << scaledSpeed << endl;
		string packet = ost.str();
		//cout << packet;
		port->writeSerial(packet);
		//cout << endl << "Number:" << pinNumber << " Speed:" << speed << endl;

	}
}
