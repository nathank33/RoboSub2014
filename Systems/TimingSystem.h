#ifndef _TSYSTEM
#define _TSYSTEM

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

using namespace boost::posix_time;

namespace Nautilus
{
	/*
		Class: TimingSystem
		Author: Nathan Braun-Krafft
		Description: The TimingSystems provide functionality for keeping track of the
			current time. TimingSystems can return values representing seconds, mins, weekday,
			monthday, month, and years. TimingSystems can also force threads to pause and provide a 
			simple true/false timer that can be set by minutes, seconds, and milliseconds.
		Example:
			int main()
			{
				TimingSystem timer;
				cout << "The current year is 20" << timer.getYears() << endl;
				cout << "Waiting 5 seconds..." << endl;
				timer.wait(5000);
				cout << "The amount of milliseconds since January 1st, 1970 is:" << timer.getMilliseconds() << endl;
				while(true)
				{
					if(!timer.isTimerActive())
					{
						cout << "This statement will run every 3.5 seconds << endl;
						timer.setTimerSeconds(3.5);
						//Also acceptable would be: timer.setTimerBoth(0,3,500)
						//							timer.setTimerMilliseconds(3500);						
					}
				}
			}
	*/
	class TimingSystem
	{
		private:
			ptime currentTime;
			tm currentTimeTM;
			ptime timer;
				
		public:
			TimingSystem();
			double getMilliseconds();
			double getSeconds();
			double getMinutes();
			double getHours();
			double getDays();
			double getMDays();
			double getMonths();
			double getYears();
			std::string getTimeStampFull();
			std::string getTimeStampHalf();
			bool isTimerActive();
			void updateTime();
			void setTimerMilliseconds(int milsec);
			void setTimerSeconds(int sec);
			void setTimerSeconds(double sec);
			void setTimerMinutes(int min);
			void setTimerBoth(int min, int sec);
			void setTimerMSM(int min, int sec, int milsec);
			void wait(int milSec);

	};
}
#endif