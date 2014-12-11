#ifndef _GATETASK
#define _GATETASK

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
    class GateTask
	{
		private:
			LoggingSystem *logger;
			RoboCamera *leftCamera;
			MovementSystem *movementSystem;

		public:
			GateTask();
			GateTask(LoggingSystem *ls, RoboCamera *cam1);
			GateTask(LoggingSystem *ls, RoboCamera *cam1, MovementSystem *ms);
			void start();
			void taskMain();
			vector<Point> getGateMidPoint();
			bool isAVerticalBar(vector<Point> singleContour);
			bool isTaskFinished(vector<Point> gatePoints);
			void moveSubToPointDegrees(Point singlePoint);
	};
}

#endif