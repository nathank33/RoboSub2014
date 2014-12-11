#ifndef _SCOMMUNICATOR
#define _SCOMMUNICATOR
#include <string>
#include "RoboPort.h"
#include "MovementSystem.h"
#include "LoggingSystem.h"
using std::string;

namespace Nautilus{

	const int pitchLight = 1;
	const int rollLight = 2;
	const int temperatureLight = 3;	

	class SerialCommunicator
	{
		private:
			RoboPort *imuPort;
			MovementSystem *movementSystem;
			LoggingSystem *logger;
			TimingSystem timer;

			double yaw;
			double pitch;
			double roll;
			bool paused;
			
			boost::thread *threadImu;
			boost::mutex mutexPauseImu;
			void startImuHelper();

		public:
			SerialCommunicator(RoboPort *imu, MovementSystem *ms, LoggingSystem *lSystem);
			
			void start(bool isThreaded = true);
			void setPaused(bool b);
			bool isPaused();

			void updateMotors();
			double getYaw();
			double getPitch();
			double getRoll();
	};
}

#endif