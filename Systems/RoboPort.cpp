// SerialCommTest1.cpp : main project file.

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include "TimingSystem.h"
#include "RoboPort.h"

using namespace std;
using namespace boost;

namespace Nautilus{
	RoboPort::RoboPort(string portName, int bRate):io(),serial(io)
	{
		/*
			Attemps to open a RoboPort object with the specified port name and baudrate.
			The program will crash if the port opening failed.
		*/
		baudRate = bRate;
		cout << "Attempting to Open Port." << endl;
		serial.open(portName);

		if(serial.is_open())
		{
			cout << "Port opened." << endl;
			serial.set_option(asio::serial_port_base::baud_rate(baudRate));
		}
		else cout << "Port did not open correctly." << endl;
	}
	void RoboPort::startReader(bool isThreaded)
	{
		/*
			Calls the port to start couting lines of input, but opens up the refresher into a new boost::thread.
			You can manually specify non multi-threading by calling start(false);
		*/
		if(isThreaded)
			threadReader = new boost::thread(boost::bind(&RoboPort::startReaderHelper,this));
		else
			startReaderHelper();
	}
	void RoboPort::startReaderHelper()
	{
		while(true)
		{
			if(!getPaused())
				cout << getPortNumber() << readLine() << endl;
		}
	}
	void RoboPort::pause(bool b)
	{
		mutexPause.lock();
		isPaused = b;
		mutexPause.unlock();
	}
	bool RoboPort::getPaused()
	{
		mutexPause.lock();
		bool t = isPaused;
		mutexPause.unlock();
		return t;
	}
	void RoboPort::close()
	{
		/*
			Closes the current serial.
		*/
		serial.close();
	}
	void RoboPort::writeSerial(string t)
	{
		/*
			Writes a string to the serial port.
		*/
		string msg = t;
		if(!serial.is_open())
		{
			cout << "Port could not be opened." << endl;
		}
		else 
		{
			mutexWriting.lock();
			boost::asio::write(serial,boost::asio::buffer(msg.c_str(),msg.size()));
			mutexWriting.unlock();
		}
	}
	void RoboPort::writeSerial(int t)
	{
		/*
			Writes an int to the serial port.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerial(msg);
	}
	void RoboPort::writeSerial(double t)
	{
		/*
			Writes a double to the serial port.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerial(msg);
	}
	void RoboPort::writeSerial(char t)
	{
		/*
			Writes a char to the serial port.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerial(msg);
	}
	void RoboPort::writeSerialln(string t)
	{
		/*
			Writes a string to the serial port and adds a return character at the end of the string.
		*/
		t += '\n';
		writeSerial(t);
	}
	void RoboPort::writeSerialln(int t)
	{
		/*
			Writes an int to the serial port and adds a return character at the end of the string.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerialln(msg);
	}
	void RoboPort::writeSerialln(double t)
	{
		/*
			Writes a double to the serial port and adds a return character at the end of the string.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerialln(msg);
	}
	void RoboPort::writeSerialln(char t)
	{
		/*
			Writes a char to the serial port and adds a return character at the end of the string.
		*/
		std::ostringstream ost;
		ost << t;
		string msg = ost.str();
		writeSerialln(msg);
	}
	string RoboPort::readLine()
	{
		/*
			Reads a line of input from the Serial port,
			this function will continue reading input until it finds a '\n'.
			When reading input with a Port, it is best to use it on a seperate thread 
			so that the rest of the program can run independently while the port is reading.
		*/
		mutexReader.lock();
		char c = ' ';
		string message;
		while (c != '\n')
		{
			asio::read(serial, asio::buffer(&c,1));
			message += c;
		}
		mutexReader.unlock();
		return message;
	}
	char RoboPort::readChar()
	{
		/*
			Reads a character from the port.
		*/
		mutexReader.lock();
		char c = ' ';
		asio::read(serial, asio::buffer(&c,1));
		mutexReader.unlock();
		return c;
	}
	string RoboPort::getPortNumber(){
		return portNumber;
	}
	int RoboPort::getBaudRate(){
		return baudRate;
	}
}