#ifndef _TMANAGER
#define _TMANAGER

#include <iostream>
#include <boost\thread.hpp>
#include "RoboCamera.h"
#include "SerialCommunicator.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include "LoggingSystem.h"


namespace Nautilus
{
	class TaskManager
	{
		protected:
			LoggingSystem* logger;
			RoboCamera* leftCamera;
			RoboCamera* bottomCamera;
			MovementSystem* movementSystem;
			
		public:
			TaskManager();
			TaskManager(RoboCamera* cam1, LoggingSystem* ls);
			TaskManager(RoboCamera* cam1, RoboCamera* cam2, MovementSystem* ms,LoggingSystem* ls);
			void startGateTask();
			void startFirstMarkerTask();
			
	};
}

#endif