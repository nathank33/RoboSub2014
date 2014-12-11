#ifndef _RMOTOR
#define _RMOTOR
#include "RoboPort.h"
#include "TimingSystem.h"
using std::string;
namespace Nautilus
{
	/*
		Class: RoboMotor
		Author: Nathan Braun-Krafft
		Description: The RoboMotor contains functionality to control a pinNumber on an Arduino.
			Basically the RoboMotor is just a collection of variables that can be added together to
			create a finalSpeed. Not all of the variables have to be used, but it is a way to seperate
			specific values which can be accessed with the respective .get methods.
				port -			A pointer to the RoboPort of the arduino.
				pinNumber -		The pin that the motor is located on in the arduino.
				previousSpeed - The previousSpeed speed that the motor had before calling any of the .set methods, used for accelerating the motors smoothly.
		Example:
			int main()
			{
				RoboPort port("COM7",57600);
				RoboMotor motor1(port,13); //Pin 13
				ostringstream ost;
				string input;
				double speed;

				while(true)
				{
					//Read input from the port and set the Y speed of the motor to that input
					input = port.readLine();
					ost << input;
					ost >> speed;
					motor1.setYSpeed(speed);
				}
				return 1;
			}
	*/
	class RoboMotor
	{
		private:
			RoboPort* port;
			int pinNumber;
			double xSpeed;
			double ySpeed;
			double zSpeed;
			double turnSpeed;
			double pitchSpeed;
			double rollSpeed;
			double yawSpeed;
			double previousSpeed;
			TimingSystem timer;
			boost::mutex mutexXSpeed;
			boost::mutex mutexYSpeed;
			boost::mutex mutexZSpeed;
			boost::mutex mutexPitchSpeed;
			boost::mutex mutexRollSpeed;
			boost::mutex mutexYawSpeed;
			boost::mutex mutexTurnSpeed;
			boost::mutex mutexPreviousSpeed;
			boost::mutex mutexAccelerateMotor;

		public:
			RoboMotor();
			RoboMotor(RoboPort *p, int pinNumber);
			RoboMotor(RoboPort *p, int pinNumber, double minSpeedPositive, double maxSpeedPositive, double minSpeedNegative, double maxSpeedNegative, double speedOff, double buoySpeed);
			
			int		getPinNumber();
			void	setXSpeed(double t);
			void	setYSpeed(double t);
			void	setZSpeed(double t);
			void	setTurnSpeed(double t);
			void	setPitchSpeed(double t);
			void	setRollSpeed(double t);
			void	setYawSpeed(double t);
			void	setPreviousSpeed(double t);

			double	getTotalSpeed();
			double	getXSpeed();
			double	getYSpeed();
			double	getZSpeed();
			double	getTurnSpeed();
			double	getPitchSpeed();
			double	getRollSpeed();
			double	getYawSpeed();
			double	getPreviousSpeed();

			void	accelerateMotorSmoothly();
			void	writeToPort(double speed);
	};
}

#endif