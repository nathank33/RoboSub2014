#ifndef _JSTICKCONTROLLER
#define _JSTICKCONTROLLER

#include <iostream>
#include <boost\thread.hpp>
#include "MovementSystem.h"
#include "TimingSystem.h"

using namespace std;

namespace Nautilus
{
	class JoystickController
	{
		/*
			Class: JoystickController
			Author: Nathan Braun-Krafft
			Note: This class should not be compiled on the beaglebone or in linux since it uses
			<Windows.h>.
		*/
	private:
		short key_1,key_2,key_3,key_4,key_5,key_6,key_7,key_8,key_A,key_B,key_C,key_D,key_E;
		int currentSpeed;
		bool paused;
		MovementSystem *ms;
		TimingSystem timer;

		boost::thread *mainThread;
		boost::mutex mutexPause;		

		void startHelper();

	public:
		JoystickController(MovementSystem *ms);
		void start(bool isThreaded = true);
		void pause(bool paused);
		bool isPaused();
	};
}
#endif