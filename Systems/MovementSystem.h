#ifndef _MSYSTEM
#define _MSYSTEM

#include <string>
#include "RoboPort.h"
#include "RoboMotor.h"
#include "LoggingSystem.h"

namespace Nautilus{
	class MovementSystem
	{
		private:
			//Motor Objects are created in the main function
			RoboMotor *fLeft;
			RoboMotor *fMid;	
			RoboMotor *fRight;	
			RoboMotor *bLeft;
			RoboMotor *bMid;	
			RoboMotor *bRight;	
			LoggingSystem *logger;
			//These variables are used to keep track of the speeds that have been given
			double xSpeed;
			double ySpeed;
			double zSpeed;
			double strafeSpeed;
			double turnSpeed;
			double yawSpeed;
			double pitchSpeed;
			double rollSpeed;
			boost::mutex mutexXSpeed;
			boost::mutex mutexYSpeed;
			boost::mutex mutexZSpeed;
			boost::mutex mutexTurnSpeed;
			boost::mutex mutexPitchSpeed;
			boost::mutex mutexYawSpeed;
			boost::mutex mutexRollSpeed;

		public:
			MovementSystem(RoboMotor *fLeft, RoboMotor *fMid, RoboMotor *fRight, RoboMotor *bLeft, RoboMotor *bMid, RoboMotor *bRight, LoggingSystem *ls);
			//Speeds run on a scale of 1 - 3 and -1 - -3
			void turn(double speed);
			void moveX(double speed);
			void moveY(double speed);
			void moveZ(double speed);
			double getXSpeed();
			double getYSpeed();
			double getZSpeed();
			double getTurnSpeed();
			double getYawSpeed();
			double getPitchSpeed();
			double getRollSPeed();

			void adjustPitch(double speed);
			void adjustRoll(double speed);
			void adjustYaw(double speed);
			void logAll();
	};
}

#endif