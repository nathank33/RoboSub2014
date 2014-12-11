#ifndef _RPORT
#define _RPORT

#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
using std::string;
namespace Nautilus{
	/*
		Class: RoboPort
		Author: Nathan Braun-Krafft
		Description: RoboPorts provide a system independent method of opening Serial Communication Ports with other communicative systems.
			RoboPorts are initialized with a port name representing the serial port location, and the specified baudrate.
		Example:
			int main()
			{
				RoboPort port("COM8",57600);
				cout << "Reading a character: " << port.readChar() << endl;
				cout << "Reading an entire line: " << port.readLine() << endl;
				cout << "Writing a string to the port:" << endl;
				port.writeSerialln("Test Line");

				cout << "Writing 1-10 in a single line with spaces:" << endl;
				ostringstream ost;
				for(int i = 1; i <= 10; i++)
					ost << i << " ";
				ost << endl;
				port.writeSerialln(ost.str());
				//NOTE: In the future the << operators might be overloaded so that ostringstreams are not necessary for concatenation.
				return 1;
			}
	*/
	class RoboPort
	{
		private:
			string portNumber;
			int baudRate;
			bool isPaused;
			boost::asio::io_service io;
			boost::asio::serial_port serial;
			boost::thread *threadReader;
			boost::mutex mutexWriting;
			boost::mutex mutexReader;
			boost::mutex mutexPause;
			void startReaderHelper();

		public:
			RoboPort(string portName, int bRate);
			
			void startReader(bool isThreaded = true);
			void pause(bool b);
			bool getPaused();

			void close();
			void writeSerial(string t);
			void writeSerial(int t);
			void writeSerial(double t);
			void writeSerial(char t);
			void writeSerialln(string t);
			void writeSerialln(int t);
			void writeSerialln(double t);
			void writeSerialln(char t);
			string readLine();
			char readChar();

			string getPortNumber();
			int getBaudRate();
	};
}

#endif