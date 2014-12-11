#define NOMINMAX
#include <boost/thread/thread.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include "RoboCamera.h"
#include "LoggingSystem.h"
#include "Tools.h"

using namespace cv;
using std::cout;
using std::endl;
using boost::thread;

namespace Nautilus
{
	RoboCamera::RoboCamera(int cameraNumber) 
		: cameraNumber(cameraNumber),cameraLocation(""),firstWindowName("Window"), logger()
	{
		init();
	}
		RoboCamera::RoboCamera(int cameraNumber, LoggingSystem* ls) 
		: cameraNumber(cameraNumber),cameraLocation(""),logger(ls),firstWindowName("Window")
	{
		/*
			This constructor allows the camera to be initialized to a camera plugged directly into the computer.
		*/
		init();
	}
	RoboCamera::RoboCamera(int cameraNumber, LoggingSystem* ls, char* firstWindowName) 
		: cameraNumber(cameraNumber),cameraLocation(""),logger(ls),firstWindowName(firstWindowName)
	{
		/*
			This constructor allows the camera to be initialized to a camera plugged directly into the computer.
		*/
		init();
	}
	RoboCamera::RoboCamera(string cameraLocation, LoggingSystem* ls, char* firstWindowName) 
		: cameraNumber(-1),cameraLocation(cameraLocation),logger(ls), firstWindowName(firstWindowName)
	{
		/*
			This constructor allows the camera to be initialized to a video file.
		*/
		init();
	}
	void RoboCamera::init()
	{
		/*
			This function is a helper function used to initialize the constructors.
		*/
		refreshCount = 0;
		setRefreshDelay(15);
		setRecordRate(30);
		setDisplayInWindow(false);
		setRecordVideo(false);
		pause(false);
	}
	void RoboCamera::start(bool isThreaded)
	{
		/*
			Calls the camera to start refreshing images, but opens up the refresher into a new boost::thread.
			You can manually specify non multi-threading by calling start(false);
		*/
		if(isThreaded)
			threadRefresh = new boost::thread(boost::bind(&RoboCamera::startHelper,this));
		else
			startHelper();
	}
	void RoboCamera::stop()
	{
		/*
			Stops a RoboCamera from running and it releases the capture device that it was holding onto.
			Do not try to start the RoboCamera after you call the Stop method, you will get an opencv
			error.
			WARNING: This function shouldn't be used if you plan to open the camera back up, use Pause instead.
		*/
		threadRefresh->interrupt();
		capture.~VideoCapture();
		delete threadRefresh;
		threadRefresh = NULL;		
	}
	void RoboCamera::pause(bool b){mutexPause.lock(); isPaused = b; mutexPause.unlock();} //Pauses and unpauses the camera.

	void RoboCamera::startHelper()
	{
		/*
			The starter is what actually forces the camera to acquire frames, the reason why
			the camera is not initially started when this object is called is so that the
			camera can be opened on a seperate thread with Boost::thread.
			The refresher will handle all videorecording to file if recordVideo has been set to true.			
		*/
		try
		{
			if(cameraNumber != -1)
				capture.open(cameraNumber);
			else if(cameraLocation != "")
				capture.open(cameraLocation);

			while(1)
			{
				if(!getPaused())
				{
					if(!capture.isOpened()){
						if(logger->isOpened())
							logger->printAndLogLn("The Capture could not be opened.");
						return;
					}
					mutexWidthHeight.lock();
					mutexImage.lock();
					capture >> regularImage;
					mutexImage.unlock();
					mutexWidthHeight.unlock();
			
					//If the variable to display in window is true then we will open a window and show our image.
					if(getDisplayInWindow())
					{
						namedWindow(firstWindowName);
						imshow(firstWindowName,regularImage);
					}

					/*
						Attempt to save the frames to a VideoWriter object. If the VideoWriter is not
						opened then we will attempt to open it, and if it still won't open log an error.
					*/
					if(getRecordVideo())
					{
						if(!videoWriterRegular.isOpened())
						{
							if(logger->isOpened())
								openRecorder();
						}
				
						if(!videoWriterRegular.isOpened())
						{
							if(logger->isOpened())
								logger->printAndLogLn("VideoWriterRegular could not be opened after multiple attempts.");
							setRecordVideo(false);
						}
						else
							videoWriterRegular << regularImage;
					}

					mutexRefreshCount.lock();
					refreshCount++;
					mutexRefreshCount.unlock();

					int waitKey = cvWaitKey(getRefreshDelay());
					if(waitKey == 27)
					{
						closeRecorder();
						stop();
						return;
					}

					boost::this_thread::interruption_point();
				}
			}
		}
		catch(const boost::thread_interrupted&){}
	}

	int RoboCamera::getWidth(){return width;} //Returns the width of the frames being obtained by the camera.
	int RoboCamera::getHeight(){return height;} //Returns the height of the frames being obtained by the camera.
	int RoboCamera::getRefreshDelay(){int t; mutexRefreshDelay.lock(); t = refreshDelay; mutexRefreshDelay.unlock(); return t;} //Access the current refreshDelay
	int RoboCamera::getRefreshCount(){int t; mutexRefreshCount.lock(); t = refreshCount; mutexRefreshCount.unlock(); return t;}	//Access the total amount of frames that have been processed
	int RoboCamera::getRecordRate(){int t; mutexRecordRate.lock(); t = recordRate; mutexRecordRate.unlock(); return t;}	//Access the total amount of frames that have been processed
	bool RoboCamera::getDisplayInWindow(){bool t; mutexDisplayInWindow.lock(); t = displayInWindow; mutexDisplayInWindow.unlock(); return t;} //Returns true if the frames are being displayed in a window.
	bool RoboCamera::getPaused(){bool t; mutexPause.lock(); t = isPaused; mutexPause.unlock(); return t;}
	bool RoboCamera::getRecordVideo(){bool t; mutexRecordVideo.lock(); t = recordVideo; mutexRecordVideo.unlock(); return t;}
	Mat RoboCamera::getImage(){Mat t; mutexImage.lock(); t = regularImage; mutexImage.unlock(); return t;} //Gets the most recent image that has been processed by the camera.
	
	//Mutex Mutator functions
	void RoboCamera::setDisplayInWindow(bool b){mutexDisplayInWindow.lock(); displayInWindow = b; mutexDisplayInWindow.unlock();} //Force the object to start displaying the frames in a window.
	void RoboCamera::setRefreshDelay(int t){mutexRefreshDelay.lock(); refreshDelay = t; mutexRefreshDelay.unlock();} //Set how fast the camera should refresh.
	void RoboCamera::setRecordVideo(bool t){mutexRecordVideo.lock(); recordVideo = t; mutexRecordVideo.unlock();} //Set if the camera should start saving the video to file with the logging object.
	void RoboCamera::setRecordRate(int t){mutexRecordRate.lock(); recordRate = t; mutexRecordRate.unlock();} //Set if the camera should start saving the video to file with the logging object.
	void RoboCamera::setWidth(int t)
	{
		/*
			Attempts to set the Width for the camera. 
			Note: Does not work with all cameras, for example it doesn't work on my Laptop
				but it does work with the C920.
		*/
		mutexWidthHeight.lock();
		width = t;
		capture.set(CV_CAP_PROP_FRAME_WIDTH,width);
		mutexWidthHeight.unlock();
	}
	void RoboCamera::setHeight(int t)
	{
		/*
			Attempts to set the Height for the camera. 
			Note: Does not work with all cameras, for example it doesn't work on my Laptop
				but it does work with the C920.
		*/
		mutexWidthHeight.lock();
		height = t;
		capture.set(CV_CAP_PROP_FRAME_HEIGHT,height);
		mutexWidthHeight.unlock();
	}

	void RoboCamera::openRecorder()
	{
		/*
			Sets the camera to start saving all of the videos to file.
			If the files have not been saving correctly we need to make sure that the directories have
			already been created.
		*/
		if(!logger->isOpened())
		{
			throw logic_error("The LoggingSystem was never opened.");
			return;
		}
		string directory = logger->getDirectory() + "Videos\\" + timer.getTimeStampFull() + " Regular.avi";
		logger->printAndLogLn(directory);
		videoWriterRegular.open(directory,CV_FOURCC('D','I','V','X'),getRecordRate(),regularImage.size(),true);
		setRecordVideo(true);
	}
	void RoboCamera::closeRecorder()
	{
		/*
			Destroys any recording devices that are active, and saves the video to file.
		*/
		logger->printAndLogLn("Closing the VideoRecorder");
		setRecordVideo(false);
		videoWriterRegular.~VideoWriter();
	}

}
