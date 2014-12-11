#include <iostream>
#include <boost\thread.hpp>
#include "RoboCamera.h"
#include "SerialCommunicator.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include "LoggingSystem.h"
#include "FirstMarkerTask.h"
#include <math.h>
#include <Windows.h>

using namespace std;

namespace Nautilus
{
	FirstMarkerTask::FirstMarkerTask()
	{

	}
	FirstMarkerTask::FirstMarkerTask(LoggingSystem *ls, RoboCamera *cam1, MovementSystem *ms)
	{
		logger = ls;
		bottomCamera = cam1;
		movementSystem = ms;
	}

	void FirstMarkerTask::start()
	{
		/*bottomCamera->setLowerThreshold(0,100,35);
		bottomCamera->setUpperThreshold(30,240,240);
		bottomCamera->setErode(1);
		bottomCamera->setDilate(6);
		bottomCamera->setRefreshRate(1);*/
		taskMain();
	}
	void FirstMarkerTask::taskMain()
	{
		/*Point largestMidPoint = bottomCamera->getLargestContourMidPoint();
		Point largestMidPointDegrees = bottomCamera->getPointToDegrees(largestMidPoint);
		bool movementStatus = movementSystem->moveSubToPointDegreesFlat(largestMidPointDegrees);
		if(movementStatus == 0) //If the sub did not move (It is in a good location);
		{
			//double pathOrientation = getPathOrientation();
		}*/
	}
	/*double FirstMarkerTask::getPathOrientation()
	{
		vector<vector<Point>> organizedContours = bottomCamera->organizeContoursByY(bottomCamera->getContours());
		if(organizedContours.size() == 1)
		{
			//sort the contour points by Y
			bool keepGoing = true;
			const double searchPercentage = 1;
			while(keepGoing)
			{
				keepGoing = false;
				for(unsigned int i = 0; i < organizedContours[0].size() - 1; i++)
				{
					if(organizedContours[0][i].y < organizedContours[0][i+1].y)
					{
						keepGoing = true;
						Point tempPoint = organizedContours[0][i];
						organizedContours[0][i] = organizedContours[0][i+1];
						organizedContours[0][i+1] = tempPoint;
					}
				}
			}
			Point topMidPoint;
			Point bottomMidPoint;
			int halfSize = organizedContours[0].size() / 6;
			for(int i = 0; i < halfSize; i++)
			{
				topMidPoint.x += organizedContours[0][organizedContours[0].size() - i - 1].x;
				topMidPoint.y += organizedContours[0][organizedContours[0].size() - i - 1].y;
				bottomMidPoint.x += organizedContours[0][i].x;
				bottomMidPoint.y += organizedContours[0][i].y;
			}
			topMidPoint.x /= halfSize;
			topMidPoint.y /= halfSize;
			bottomMidPoint.x /= halfSize;
			bottomMidPoint.y /= halfSize;
			double oppositeLine = topMidPoint.y - bottomMidPoint.y * -1;
			double adjacentLine = topMidPoint.x - bottomMidPoint.x;
			double theta = atan(oppositeLine / adjacentLine) * (180 / PI);
			if(theta < 0)
				theta = theta *-1 - 90;
			else theta = 90 - theta;
			cout << "Opposite: " << oppositeLine << "  Adjacent: " << adjacentLine << " Theta: " << theta << endl;
			return theta;
		}	
	}*/
}