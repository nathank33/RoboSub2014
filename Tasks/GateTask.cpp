#include <iostream>
#include <boost\thread.hpp>
#include "RoboCamera.h"
#include "SerialCommunicator.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include "LoggingSystem.h"
#include "Tools.h"
#include "GateTask.h"
#include <math.h>
#include <Windows.h>

using namespace std;

namespace Nautilus
{
	GateTask::GateTask()
	{

	}
	GateTask::GateTask(LoggingSystem *ls, RoboCamera *cam1)
	{
		logger = ls;
		leftCamera = cam1;
	}
	GateTask::GateTask(LoggingSystem *ls, RoboCamera *cam1, MovementSystem *ms)
	{
		logger = ls;
		leftCamera = cam1;
		movementSystem = ms;
	}

	void GateTask::start()
	{
		leftCamera->setLowerThreshold(0,100,35);
		leftCamera->setUpperThreshold(30,240,240);
		leftCamera->setErode(1);
		leftCamera->setDilate(6);
		leftCamera->setRefreshRate(5);
		leftCamera->setMinimumContourArea(1000);
		taskMain();
	}
	void GateTask::taskMain()
	{
		logger->printAndLogLn("Searching for the middle of the gate.");
		const int TASK_FINISH_AMOUNT_OF_CHECKS = 15;
		int taskFinishedCheckCounter = 0;
		bool taskFinished = false;
		
		while(!taskFinished)
		{
			vector<vector<Point>> contoursCopy = leftCamera->getContours();
			vector<Point> gateLocationPoints = getGateMidPoint();
			Point gateMidPoint = gateLocationPoints[0];
			vector<vector<Point>> testContours;
			testContours.push_back(gateLocationPoints);
			if(isTaskFinished(gateLocationPoints))
				taskFinishedCheckCounter++;
			if(taskFinishedCheckCounter > TASK_FINISH_AMOUNT_OF_CHECKS)
				taskFinished = true;
			Point gateMidPointDegrees = Tools::getPointToDegrees(gateMidPoint,leftCamera->getWidth(),leftCamera->getHeight());
			movementSystem->moveSubToPointDegreesUpward(gateMidPointDegrees);

			Sleep(20);
		}
		cout << "Task Finished" << endl;
	}
	vector<Point> GateTask::getGateMidPoint() //Returns a vector that contains the Gate Midpoint[0], Left Bar[1], and Right Bar[2]
	{
		const double DIFFERENCE_OF_AREAS_THRESHOLD = 4000; //based on ContourArea
		double differenceBetweenAreas = 0;
		vector<vector<Point>> contours = leftCamera->getContours();
		vector<Point> gatePoints;
		Point gateMidPoint(-1,-1);
		Point gateLeftBar(-1,-1);
		Point gateRightBar(-1,-1);
		if(contours.size() == 0)
		{
			logger->printAndLogLn("No contours found.");
		}
		if(contours.size() == 1)
		{
			if(isAVerticalBar(contours[0]))
			{
				logger->printAndLogLn("We found 1 vertical bar.");
			}
			else
			{
				logger->printAndLogLn("We found 1 contour, but it was not a vertical bar.");
			}
		}
		else if(contours.size() > 1) //There are too many contours and we need to merge and remove some
		{
			//Check to see if we see at least 2 vertical bars in the mess of contours that we found
			vector<vector<Point>> totalBarsFound;
			for(unsigned int i = 0; i < contours.size(); i++)
			{
				if(isAVerticalBar(contours[i]))
					totalBarsFound.push_back(contours[i]);
			}
			if(totalBarsFound.size() == 0)
			{
				logger->printAndLogLn("No vertical bars found.");
			}
			else if(totalBarsFound.size() == 1)
			{
				logger->printAndLogLn("Only found 1 vertical bar.");
			}
			else if(totalBarsFound.size() == 2)
			{
				
				differenceBetweenAreas = abs(contourArea(contours[0]) - contourArea(contours[1]));
				if(differenceBetweenAreas > DIFFERENCE_OF_AREAS_THRESHOLD)							//CHECK FOR THIS LATER
				{
					logger->printAndLogLn("One of the vertical bars is much bigger than the other");
				}
				else
				{
					logger->printAndLogLn("Two of the contours were vertical bars.");
					totalBarsFound = Tools::organizeByX(totalBarsFound);
					gateLeftBar = Tools::getMidPoint(totalBarsFound[0]);
					gateRightBar = Tools::getMidPoint(totalBarsFound[1]);
					gateMidPoint.x = (Tools::getMidPoint(totalBarsFound[0]).x + Tools::getMidPoint(totalBarsFound[1]).x) / 2;
					gateMidPoint.y = (Tools::getMidPoint(totalBarsFound[0]).y + Tools::getMidPoint(totalBarsFound[1]).y) / 2;
					
				}
			}
			else if(totalBarsFound.size() > 2)
			{
				logger->printAndLogLn("Warning!   Found more than 2 vertical bars.");
			}
		}
	gatePoints.push_back(gateMidPoint);
	gatePoints.push_back(gateLeftBar);
	gatePoints.push_back(gateRightBar);
	return gatePoints;
	}
	bool GateTask::isAVerticalBar(vector<Point> singleContour)
	{
		double heightToWidthRatio = 3; //Height should be at least this times as big
		vector<double> heightAndWidth = Tools::getHeightAndWidth(singleContour);
		double hAndWRatio = heightAndWidth[0] / heightAndWidth[1];
		if(hAndWRatio > heightToWidthRatio) 
			return true;
		else 
			return false;
	}
	bool GateTask::isTaskFinished(vector<Point> gatePoints)
	{
		const double DEGREE_DISTANCE_FOR_FINISH = 180;
		Point leftBarDegrees = Tools::getPointToDegrees(gatePoints[1],leftCamera->getHeight(),leftCamera->getWidth());
		Point rightBarDegrees = Tools::getPointToDegrees(gatePoints[2],leftCamera->getHeight(),leftCamera->getWidth());
		double degreeDifferenceBetweenBars = abs(leftBarDegrees.x - rightBarDegrees.x);
		if(degreeDifferenceBetweenBars > DEGREE_DISTANCE_FOR_FINISH)
			return true;	
		else return false;
	}
	void GateTask::moveSubToPointDegrees(Point singlePoint)
	{
		const double horizontalMovementMultiplier = 1; //Per degree
		const double verticalMovementMultiplier = 1;
		double xThreshold = 6;
		double yThreshold = 6;
		bool needToAdjust = false;

		std::ostringstream logString;
		logString << "Middle point found at X: " << singlePoint.x << '\t' << "Y: " << singlePoint.y;
		logger->printAndLogLn(logString.str());

		if(singlePoint.x < (90 - xThreshold))
		{
			needToAdjust = true;
			logger->printAndLogLn("Strafing left to reach the point");
		}
		else if(singlePoint.x > (90 + xThreshold))
		{
			needToAdjust = true;
			logger->printAndLogLn("Strafing right to reach the point.");
		}
		if(singlePoint.y < (90 - yThreshold)) //The gate's midpoint is too High
		{
			needToAdjust = true;
			logger->printAndLogLn("Moving upward to reach the point.");
		}
		else if(singlePoint.y > (90 + yThreshold))
		{
			needToAdjust = true;
			logger->printAndLogLn("Moving down to reach the point.");
		}
		if(!needToAdjust)
		{
			logger->printAndLogLn("Submarine position is acceptable, proceeding.");
		}
	}
}