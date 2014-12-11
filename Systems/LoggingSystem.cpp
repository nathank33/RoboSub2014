#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <opencv2/highgui/highgui.hpp>
#include <boost/thread/thread.hpp>
#include "LoggingSystem.h"
#include "TimingSystem.h"
#include "Tools.h"

using namespace std;

namespace Nautilus{
	LoggingSystem::LoggingSystem(string loc)
	{
		/*
			The constructor opens the LoggingSystem with the specified string.
		*/
		open(loc);
	}
	LoggingSystem::LoggingSystem(){}

	void LoggingSystem::open(string loc)
	{
		/*
			Init is the helper function to the constructors. It simply constructs
			the object based on the directories specified.
		*/
		std::ostringstream ost;
		ost << tSystem.getTimeStampFull() << ".txt";
		initialTimeStamp = ost.str();
		directoryLocation = loc;
		string fileName = directoryLocation + initialTimeStamp;

		/*
		 * If the file already exists then we need to change the filename,
		 * We do not want to override a preexisting log file.
		 */
		if(Tools::doesPathExist(fileName))
			Tools::addDuplicationToPath(fileName);

		//If the file was already open then we should close it before opening another file.
		if(outputFile.is_open())
			outputFile.close();

		outputFile.open(fileName.c_str());
		if(outputFile.is_open())
		{
			opened = true;
			printAndLogLn("The file has been opened.");
		}
		else 
		{
			opened = false;
			cout << "The file could not be opened." << endl;
		}
	}
	string LoggingSystem::getDirectory()
	{
		/*
			Returns the string directory of the LoggingSystem.
		*/
		return directoryLocation;
	}
	void LoggingSystem::logToFile(string s)
	{
		/*
			Logs a string to the file but does not print it to the screen or add the line terminator.
		*/
		if(!opened)
			throw runtime_error("The logging system has a null directory, cannot log to file.");

		mutexLog.lock();
		if (outputFile.is_open())
			outputFile << tSystem.getTimeStampHalf() << "\t" << s;
		//else cout << "File could not be opened." << endl;
		mutexLog.unlock();
	}
	void LoggingSystem::logToFileLn(string s)
	{
		/*
			Logs a string to the file but does not print it to the screen.
			Adds a Line terminator to the end of the string.
		*/
		logToFile(s);
		logToFile("\n");
	}
	void LoggingSystem::printOut(string s)
	{
		/*
			Prints a string to the screen.
			Kinda useless except that the other functions call it.
		*/
		cout << s;
	}
	void LoggingSystem::printOutLn(string s)
	{
		/*
			Prints a string with a line terminator.
		*/
		printOut(s);
		printOut("\n");
	}
	void LoggingSystem::printAndLog(string s)
	{
		/*
			Prints and logs a string without line terminators.
		*/
		logToFile(s);
		printOut(s);
	}
	void LoggingSystem::printAndLogLn(string s)
	{
		/*
			Print and log a string with line terminators.
		*/
		logToFileLn(s);
		printOutLn(s);
	}
	void LoggingSystem::logImage(cv::Mat image, string name, bool isLinux)
	{
		/*
			Saves a Mat to the directory.
		*/
		if(!opened)
			throw runtime_error("The logging system has a null directory, cannot log to file.");

		std::ostringstream ost;
		ost << tSystem.getTimeStampFull() << "  " << name << ".jpg";
		string temporaryStamp = ost.str();
		string temporaryFileName = directoryLocation + "\\Pictures\\" + temporaryStamp;
		if(isLinux)
			temporaryFileName = directoryLocation + "//Pictures//" + temporaryStamp;
		cv::imwrite(temporaryFileName,image);
	}
	bool LoggingSystem::isOpened()
	{
		return opened;
	}
}
