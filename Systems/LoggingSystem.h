#ifndef _LSYSTEM
#define _LSYSTEM

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <boost/thread/thread.hpp>
#include "TimingSystem.h"

using std::string;
using cv::Mat;

namespace Nautilus
{
	/*
		Class: LoggingSystem
		Author: Nathan Braun-Krafft
		Description: The LoggingSystem provides functionality for logging information to a directory.
			By using logToFile, logToFileLn, printAndLog, and printAndLogLn you easily write a string
			to the specified directory that was supplied in the constructor or using the .open() function.
			When a string is logged it is automatically written with a TimeStamp making it easier to keep
			track of when each line was written. In the future there may exist functionality to log the string
			without the timeStamp, but for now it is automatically included.
				outputFile -		The file that will hold the logged information
				initialTimeStamp -	The file is named based on this timestamp, it is "MO.d.y h.m.s" such as 3.10.14 6.47.15, which would be The 10th of March, 2014 at 6:47:15
				directoryLocation - The location of the ofstream file.
			Example:
				int main()
				{
					VideoCapture cap(0);
					Mat image;
					LoggingSystem logger;
					logger.open("C:\\Users\\NathanLaptop\\Desktop\\RoboSub\\Logs\\");
					logger.logToFileLn("Testing, Testing, we just opened our first LoggingSystem WOO!");
					logger.printAndLogLn("This appears in the file and in the console, Cool!");
					capture >> image;
					logger.logToFileLn("Saving the first image as "Image1");
					logger.logImage(image,"Image1");
					return;
				}
	*/
	class LoggingSystem
	{
		private:
			TimingSystem tSystem;
			std::ofstream outputFile;
			boost::mutex mutexLog;
			string initialTimeStamp;
			string directoryLocation;
			string pictureDirectory;
			bool opened;
		public:
			LoggingSystem();
			LoggingSystem(string loc);
			void open(string loc);
			string getDirectory();
			void logToFile(string s);
			void logToFileLn(string s);
			void printOut(string s);
			void printOutLn(string s);
			void printAndLog(string s);
			void printAndLogLn(string s);
			void logImage(cv::Mat image, string name, bool isLinux = false);
			bool isOpened();
	};
}



#endif
