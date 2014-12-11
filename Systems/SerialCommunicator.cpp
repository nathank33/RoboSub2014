#include <iostream>
#include "SerialCommunicator.h"
#include "RoboPort.h"
#include "LoggingSystem.h"
#include "TimingSystem.h"
#include "Tools.h"
#include <string>
#include <math.h>

using namespace std;

namespace Nautilus{
	SerialCommunicator::SerialCommunicator(RoboPort *imu, MovementSystem *ms, LoggingSystem *lSystem)
	{
		imuPort = imu;
		yaw = 0;
		pitch = 0;
		roll = 0;
		movementSystem = ms;
		logger = lSystem;
	}
	void SerialCommunicator::start(bool isThreaded)
	{
		/*
			Calls the Communicator to start reading data from the imuPort, but opens up the refresher into a new boost::thread.
			You can manually specify non multi-threading by calling start(false);
		*/
		if(isThreaded)
			threadImu = new boost::thread(boost::bind(&SerialCommunicator::startImuHelper,this));
		else
			startImuHelper();
	}
	void SerialCommunicator::setPaused(bool b)
	{
		mutexPauseImu.lock();
		paused = b;
		mutexPauseImu.unlock();
	}
	bool SerialCommunicator::isPaused()
	{
		mutexPauseImu.lock();
		bool t = paused;
		mutexPauseImu.unlock();
		return t;
	}
	void SerialCommunicator::startImuHelper()
	{
		//Take in all the information from the IMU
		//Do adjustments to make sure everything is balanced
		const int identifierSize = 5; //"#YPR="
		const int delayBetweenLoops = 5;
		string dataReceived = "";
		while(true)
		{
			if(!isPaused())
			{
				dataReceived = "";
				dataReceived = imuPort->readLine();
				
				string identifier = dataReceived.substr(0,identifierSize);
				if(identifier == "#YPR=")
				{
					Tools::replaceAll(dataReceived,identifier,"");
					vector<string> split = Tools::split(dataReceived,',');
					yaw = atof(split.at(0).c_str());
					pitch = atof(split.at(1).c_str());
					roll = atof(split.at(2).c_str());
					cout << "Y:" << yaw << "\tP:" << pitch << "\tR:" << roll << endl;
					//updateMotors();
				}
				timer.wait(delayBetweenLoops);
			}
		}
	}
	void SerialCommunicator::updateMotors()
	{
		const double pitchThreshold = 3, rollThreshold = 3;
		const double pitchRatio = 2, rollRatio = 2;
		if(abs(pitch - pitchThreshold) > 0)
			movementSystem->adjustPitch(pitch * pitchRatio);
		if(abs(roll - rollThreshold) > 0)
			movementSystem->adjustRoll(roll * rollRatio);
	}
	double SerialCommunicator::getYaw(){return yaw;}				// Returns the total Yaw from the IMU.
	double SerialCommunicator::getPitch(){return pitch;}			// Returns the total Pitch from the IMU.
	double SerialCommunicator::getRoll(){return roll;}				// Returns the total Roll from the IMU.
}