#include <iostream>
#include "RoboMotor.h"
#include "RoboCamera.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include "SerialCommunicator.h"
using namespace std;

namespace Nautilus
{
	MovementSystem::MovementSystem(RoboMotor *fLeft, RoboMotor *fMid, RoboMotor *fRight, RoboMotor *bLeft, RoboMotor *bMid, RoboMotor *bRight, LoggingSystem *ls)
	{
		this->fLeft = fLeft;
		this->fMid = fMid;
		this->fRight = fRight;
		this->bLeft = bLeft;
		this->bMid = bMid;
		this->bRight = bRight;

		logger = ls;
		xSpeed = 0;
		ySpeed = 0;
		zSpeed = 0;
		turnSpeed = 0;
	}
	void MovementSystem::logAll()
	{
		std::ostringstream ost;
		ost << "FLeft: " << fLeft->getTotalSpeed() << "\t";
		ost << "FMid: " << fMid->getTotalSpeed() << "\t";
		ost << "FRight: " << fRight->getTotalSpeed() << "\t";
		ost << "BLeft: " << bLeft->getTotalSpeed() << "\t";
		ost << "BMid: " << bMid->getTotalSpeed() << "\t";
		ost << "BRight: " << bRight->getTotalSpeed() << "\t";
		logger->printAndLog(ost.str());
	}
	//Scales of -3 to 3d
	void MovementSystem::moveX(double speed)
	{
		double absSpeed = abs(speed);
		if(speed > 0)
		{
			fLeft->setXSpeed(100 * absSpeed);
			fRight->setXSpeed(-100 * absSpeed);
			bLeft->setXSpeed(100 * absSpeed);
			bRight->setXSpeed(-100 * absSpeed);
		}
		else if(speed < 0)
		{
			fLeft->setXSpeed(-100 * absSpeed);
			fRight->setXSpeed(100 * absSpeed);
			bLeft->setXSpeed(-100 * absSpeed);
			bRight->setXSpeed(100 * absSpeed);
		}
		else if(speed == 0)
		{
			fLeft->setXSpeed(0);
			fRight->setXSpeed(0);
			bLeft->setXSpeed(0);
			bRight->setXSpeed(0);
		}
		mutexXSpeed.lock();
		xSpeed = 100 * speed;
		mutexXSpeed.unlock();
	}
	//front left sucking in air, counter clockwise
	//front right counter clock wise
	//back left, counter clockwise
	//back right, clockwise
	void MovementSystem::moveY(double speed)
	{
		double absSpeed = abs(speed);
		if(speed > 0)
		{
			fLeft->setYSpeed(-52 * absSpeed);
			fRight->setYSpeed(100 * absSpeed);
			bLeft->setYSpeed(-100 * absSpeed);
			bRight->setYSpeed(-100 * absSpeed);
		}
		else if(speed < 0)
		{
			fLeft->setYSpeed(100 * absSpeed);
			fRight->setYSpeed(-70 * absSpeed);
			bLeft->setYSpeed(100 * absSpeed);
			bRight->setYSpeed(100 * absSpeed);
		}
		else if(speed == 0)
		{
			fLeft->setYSpeed(0);
			fRight->setYSpeed(0);
			bLeft->setYSpeed(0);
			bRight->setYSpeed(0);
		}
		mutexYSpeed.lock();
		ySpeed = 100 * speed;
		mutexYSpeed.unlock();
	}
	void MovementSystem::moveZ(double speed)
	{
		double absSpeed = abs(speed);
		if(speed > 0)
		{
			fMid->setZSpeed(-100 * absSpeed);
			bMid->setZSpeed(-100 * absSpeed);
			
		}
		else if(speed < 0)
		{
			fMid->setZSpeed(100 * absSpeed);
			bMid->setZSpeed(100 * absSpeed);
			fLeft->setZSpeed(25 * absSpeed);
			fRight->setZSpeed(45 * absSpeed);
			bLeft->setZSpeed(45 * absSpeed);
			bRight->setZSpeed(-15 * absSpeed);

		}
		else if(speed == 0)
		{
			fMid->setZSpeed(0);
			bMid->setZSpeed(0);
			fRight->setZSpeed(0);
			bLeft->setZSpeed(0);
			bRight->setZSpeed(0);
			fLeft->setZSpeed(0);
		}
		mutexZSpeed.lock();
		zSpeed = 100 * speed;
		mutexZSpeed.unlock();
	}
	void MovementSystem::turn(double speed)
	{
		double absSpeed = abs(speed);
		if(speed > 0)
		{
			fLeft->setTurnSpeed(100 * absSpeed);
			fRight->setTurnSpeed(-100 * absSpeed);
			bLeft->setTurnSpeed(-100 * absSpeed);
			bRight->setTurnSpeed(100 * absSpeed);
		}
		else if(speed < 0)
		{
			fLeft->setTurnSpeed(-100 * absSpeed);
			fRight->setTurnSpeed(100 * absSpeed);
			bLeft->setTurnSpeed(100 * absSpeed);
			bRight->setTurnSpeed(-100 * absSpeed);
		}
		else if(speed == 0)
		{
			fLeft->setTurnSpeed(0);
			fRight->setTurnSpeed(0);
			bLeft->setTurnSpeed(0);
			bRight->setTurnSpeed(0);
		}
		mutexTurnSpeed.lock();
		turnSpeed = 100 * speed;
		mutexTurnSpeed.unlock();
	}

	//Scales of -100 to 100
	void MovementSystem::adjustPitch(double speed)
	{
		mutexPitchSpeed.lock();
		fMid->setPitchSpeed(speed);
		bMid->setPitchSpeed(-speed);
		mutexPitchSpeed.unlock();
	}
	void MovementSystem::adjustRoll(double speed)
	{
		mutexRollSpeed.lock();
		fLeft->setRollSpeed(speed);
		bLeft->setRollSpeed(speed);
		fRight->setRollSpeed(-speed);
		bRight->setRollSpeed(-speed);
		mutexRollSpeed.unlock();
	}
	void MovementSystem::adjustYaw(double speed)
	{
		mutexYawSpeed.lock();
		fLeft->setYawSpeed(speed);
		bLeft->setYawSpeed(-speed);
		fRight->setYawSpeed(-speed);
		bRight->setYawSpeed(speed);
		mutexYawSpeed.unlock();
	}

	double MovementSystem::getXSpeed(){mutexXSpeed.lock(); double t = xSpeed; mutexXSpeed.unlock(); return t;}
	double MovementSystem::getYSpeed(){mutexYSpeed.lock(); double t = ySpeed; mutexYSpeed.unlock(); return t;}
	double MovementSystem::getZSpeed(){mutexZSpeed.lock(); double t = zSpeed; mutexZSpeed.unlock(); return t;}
	double MovementSystem::getTurnSpeed(){mutexTurnSpeed.lock(); double t = turnSpeed; mutexTurnSpeed.unlock(); return t;}
}