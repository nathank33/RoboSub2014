#ifndef _RSERVO
#define _RSERVO
#include "RoboPort.h"
using std::string;
namespace Nautilus{
	class RoboServo
	{
		private:
			RoboPort* port;
			int pinNumber;
			int regularDegree;

		public:
			RoboServo();
			RoboServo(Nautilus::RoboPort *p, int pinNumber);
			void startMotor();
			void stopMotor();
			int getDegree();
			void setDegree(int degr);
			void adjustDegree(int degr);
			void writeToPort();
	};
}

#endif