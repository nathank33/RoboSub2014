#include "TaskManager.h"
#include "GateTask.h"
#include "FirstMarkerTask.h"

using namespace std;

namespace Nautilus
{
	TaskManager::TaskManager()
	{

	}
	TaskManager::TaskManager(RoboCamera* cam1, LoggingSystem* ls)
	{
		logger = ls;
		leftCamera = cam1;
	}
	TaskManager::TaskManager(RoboCamera* cam1, RoboCamera* cam2, MovementSystem* ms,LoggingSystem* ls)
	{
		leftCamera = cam1;
		bottomCamera = cam2;
		movementSystem = ms;
		logger = ls;
		logger->printAndLogLn("Task Manager Initialized");
	}

	void TaskManager::startGateTask()
	{
		//GateTask gateTask(logger, leftCamera, movementSystem);
		GateTask gateTask(logger, leftCamera);
		logger->printAndLogLn("Starting the gateTask.");
		gateTask.start();
		leftCamera->setRefreshDelay(15);
	}
	void TaskManager::startFirstMarkerTask()
	{
		movementSystem->moveX(25);//The initial speed after the gate task to go to the first marker task
		bottomCamera->setRefreshDelay(5);
		FirstMarkerTask firstMarkerTask(logger, bottomCamera, movementSystem);
	}
}