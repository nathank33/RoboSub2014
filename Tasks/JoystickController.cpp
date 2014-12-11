
#include <iostream>
#include <boost\thread.hpp>
#include "JoystickController.h"
#include "MovementSystem.h"
#include "TimingSystem.h"
#include <Windows.h>

namespace Nautilus
{
	JoystickController::JoystickController(MovementSystem *ms)
	{
		this->ms = ms;
		currentSpeed = 1;
		key_1 = false;
		key_2 = false;
		key_3 = false;
		key_4 = false;
		key_5 = false;
		key_6 = false;
		key_7 = false;
		key_8 = false;
		key_A = false;
		key_B = false;
		key_C = false;
		key_D = false;
		key_E = false;
	}
	void JoystickController::start(bool isThreaded)
	{
		if(isThreaded)
			mainThread = new boost::thread(boost::bind(&JoystickController::startHelper,this));
		else
			startHelper();
	}
	void JoystickController::pause(bool paused)
	{
		mutexPause.lock();
		this->paused = paused;
		mutexPause.unlock();
	}
	bool JoystickController::isPaused()
	{
		mutexPause.lock();
		bool t = paused;
		mutexPause.unlock();
		return t;
	}
	void JoystickController::startHelper()
	{
		while(!isPaused())
		{
			key_1 = GetAsyncKeyState(0x31);	//1
			key_2 = GetAsyncKeyState(0x32);	//2
			key_3 = GetAsyncKeyState(0x33);	//3
			key_4 = GetAsyncKeyState(0x34);	//4
			key_5 = GetAsyncKeyState(0x35);	//5
			key_6 = GetAsyncKeyState(0x36);	//6
			key_7 = GetAsyncKeyState(0x37);	//7
			key_8 = GetAsyncKeyState(0x38);	//8
			key_A = GetAsyncKeyState(0x41);	//A
			key_B = GetAsyncKeyState(0x42);	//B
			key_C = GetAsyncKeyState(0x43);	//C
			key_D = GetAsyncKeyState(0x44);	//D
			key_E = GetAsyncKeyState(0x45);	//E

			// Strafe Right/Left Movement
			if(key_1 || key_2 || key_3 || key_4 || key_5 || key_6 
				|| key_7 || key_8 || key_A || key_B || key_C || key_D || key_E)
			{
				if(key_1){
					cout << "JoyStick 1" << endl;
					ms->moveX(currentSpeed);
				}
				else if(key_2){
					cout << "JoyStick 2" << endl;
					ms->moveX(currentSpeed * -1);
				}
				else{
					if(ms->getYSpeed() != 0){
						ms->moveX(0);
						cout << "JoyStick X Off" << endl;
					}
				}

				// Forward/Reverse Movement
				if(key_3){
					cout << "JoyStick 3" << endl;
					ms->moveY(currentSpeed);
				}
				else if(key_4){
					cout << "JoyStick 4" << endl;
					ms->moveY(currentSpeed * -1);
				}
				else{
					if(ms->getXSpeed() != 0){
						cout << "JoyStick Y Off" << endl;
						ms->moveY(0);
					}
				}

				// Up/Down Movement
				if(key_5){
					cout << "JoyStick 5" << endl;
					ms->moveZ(currentSpeed);
				}
				else if(key_6){
					cout << "JoyStick 6" << endl;
					ms->moveZ(currentSpeed * -1);
				}
				else{
					if(ms->getZSpeed() != 0){
						cout << "JoyStick Z Off" << endl;
						ms->moveZ(0);	
					}
				}

				// Turn Right/Left Movement
				if(key_7){
					cout << "JoyStick 7" << endl;
					ms->turn(currentSpeed * 1);
				}
				else if(key_8){
					cout << "JoyStick 8" << endl;
					ms->turn(currentSpeed * -1);
				}
				else{
				
					if(ms->getTurnSpeed() != 0){
						cout << "Turn off" << endl;
						ms->turn(0);
					}
				}

				// Speed Adjuster
				if(key_A){
					cout << "JoyStick A" << endl;
					currentSpeed = 1;
				}
				else if(key_B){
					cout << "JoyStick B" << endl;
					currentSpeed = 2;
				}
				else if(key_C){
					cout << "JoyStick C" << endl;
					currentSpeed = 3;
				}
				else if(key_D){
					cout << "All OFF" << endl;
					ms->moveX(0);
					ms->moveY(0);
					ms->moveZ(0);
					ms->turn(0);
				}
				timer.wait(250);
			}
		}
	}

}