#include <iostream>
#include "RoboServo.h"
#include "RoboPort.h"

using namespace std;

namespace Nautilus{

	RoboServo::RoboServo()
	{

	}

	RoboServo::RoboServo(Nautilus::RoboPort *p, int pinNumber)
	{
		port = p;
		this->pinNumber = pinNumber;
		regularDegree = 0;
	}
	
	int RoboServo::getDegree()
	{
		return regularDegree;
	}

	void RoboServo::setDegree(int deg)
	{
		regularDegree = deg;
		writeToPort();
	}

	void RoboServo::adjustDegree(int deg)
	{
		regularDegree+=deg;
		writeToPort();
	}

	void RoboServo::writeToPort()
	{
		int packet = pinNumber * 1000;
		if(regularDegree < 0) regularDegree = 0;
		if(regularDegree > 180) regularDegree = 180;
		packet+=regularDegree;
		cout << packet << endl;
		port->writeSerial(packet);
	}
}