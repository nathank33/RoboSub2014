#ifndef _FMARKERTASK
#define _FMARKERTASK

#include <iostream>
#include <boost\thread.hpp>
#include "RoboCamera.h"
#include "SerialCommunicator.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include "LoggingSystem.h"
#include "TaskManager.h"

namespace Nautilus
{
	class FirstMarkerTask
	{
		private:
			LoggingSystem *logger;
			RoboCamera *bottomCamera;
			MovementSystem *movementSystem;

		public:
			FirstMarkerTask();
			FirstMarkerTask(LoggingSystem *ls, RoboCamera *cam1, MovementSystem *ms);
			void start();
			void taskMain();
			double getPathOrientation();
	};
}

#endif