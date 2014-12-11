#ifndef CAMERA
#define CAMERA

#include "LoggingSystem.h"
using namespace cv;

namespace Nautilus{
	/*
		Class: RoboCamera
		Author: Nathan Braun-Krafft
		Description: The RoboCamera object is used to open and close the Cameras for the sub. RoboCameras
			have been in functionality for saving videos and logging them to file. The location for which
			the videos are saved is based upon the LoggingSystem parameter. All of the variables are
			defended by mutexes since each camera is most likely to be multithreaded. Once a camera has been created
			it must be called with .start() to actually force the camera to run.
				cameraNumber -		The number of the camera in the computer
				cameraLocation -	The location of a Video file if the cameraNumber is not being used.
				firstWindowName -	The name of the window if setDisplayInWindow is true.
				regularImage -		The mat that holds the cameras image.
				refreshDelay -		A hardcoded delay that can be used to slow down the camera.
				refreshCount -		Everytime a frame is processed this variable is incremented by 1.
				recordRate -		The speed at which the video will be recorded, make sure this is accurate with the FPS or else the video will be too fast or too slow.
			Example:
				int main()
				{
					LoggingSystem logger;
					logger.open("C:\\Users\\NathanLaptop\\Desktop\\RoboSub\\Logs\\");
					logger.logToFileLn("We are testing the camera");
					RoboCamera cam1(0,"Left Camera Window",&logger);
					cam1.setDisplayInWindow(true);
					cam1.setRecordVideo(true);
					cam1.setRefreshDelay(10);
					cam1.setRecordRate(30);
					cam1.start(); //Call cam1.start(false) to remove multithreading.
				}
	*/
	class RoboCamera
	{
		private:
			int cameraNumber;	
			string cameraLocation;
			char* firstWindowName;
			
			Mat regularImage;
			LoggingSystem* logger;
			VideoCapture capture;
			VideoWriter videoWriterRegular;
			TimingSystem timer;			

			int width, height;
			int refreshDelay;
			int refreshCount;
			int recordRate;

			bool displayInWindow;
			bool recordVideo;
			bool isPaused;

			boost::mutex mutexDisplayInWindow, mutexPause, mutexRecordVideo;
			boost::mutex mutexRefreshDelay, mutexRefreshCount, mutexRecordRate;
			boost::mutex mutexImage, mutexWidthHeight;
			boost::thread *threadRefresh;

			void init();
			void startHelper();
			void openRecorder();
			void closeRecorder();

		public:
			RoboCamera(int cameraNumber);
			RoboCamera(int cameraNumber, LoggingSystem* ls);
			RoboCamera(int cameraNumber, LoggingSystem* ls, char* firstWindowName);
			RoboCamera(string cameraLocation, LoggingSystem* ls, char* firstWindowName);
			
			void start(bool isThreaded = true);
			void stop();
			void pause(bool b);

			int getWidth();
			int getHeight();
			int getRefreshDelay();
			int getRefreshCount();
			int getRecordRate();
			bool getDisplayInWindow();
			bool getPaused();
			bool getRecordVideo();
			Mat getImage();

			void setWidth(int t);
			void setHeight(int t);
			void setDisplayInWindow(bool b);
			void setRefreshDelay(int t);
			void setRecordRate(int t);
			void setRecordVideo(bool b);
	};
}
#endif