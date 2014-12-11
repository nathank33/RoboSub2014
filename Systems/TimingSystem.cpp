#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include "TimingSystem.h"
#include <string>

using namespace boost::posix_time;
using namespace std;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::microsec_clock;

namespace Nautilus
{
	TimingSystem::TimingSystem()
	{
		/*
			Update the time and then initialize the timer.
		*/
		currentTime = second_clock::local_time();
		currentTimeTM = to_tm(currentTime);
		timer = currentTime;
	}
	double TimingSystem::getMilliseconds()
	{
		/*
			Returns a 13 digit timestamp that represents the amount of milliseconds since 1970.
		*/
		static ptime const epoch(date(1970, 1, 1));
		return (double)(microsec_clock::universal_time() - epoch).total_milliseconds();
	}
	double TimingSystem::getSeconds()
	{
		/*
			Returns a number representing the current second.
		*/
		updateTime();
		return currentTimeTM.tm_sec;
	}
	double TimingSystem::getMinutes()
	{
		/*
			Returns a number representing the current minute.
		*/
		updateTime();
		return currentTimeTM.tm_min;
	}
	double TimingSystem::getHours()
	{
		/*
			Returns a number representing the current hour.
		*/
		updateTime();
		return currentTimeTM.tm_hour;
	}
	double TimingSystem::getDays()
	{
		/*
			Returns the current weekday, 1-7.
		*/
		updateTime();
		return currentTimeTM.tm_wday;
	}
	double TimingSystem::getMDays()
	{
		/*
			Returns the current month day, 1-31.
		*/
		updateTime();
		return currentTimeTM.tm_mday;
	}
	double TimingSystem::getMonths()
	{
		/*
			Returns the current month, 1-12.
		*/
		updateTime();
		return currentTimeTM.tm_mon + 1;
	}
	double TimingSystem::getYears()
	{
		/*
			Returns the current year
		*/
		updateTime();
		return currentTimeTM.tm_year;
	}
	string TimingSystem::getTimeStampFull()
	{
		/*
			Returns a full timestamp of M,mo,y  h,m,s.
		*/
		std::ostringstream ost;
		ost << getMonths() << "." << getMDays() << "." << (int)getYears() % 100 << "   " << (int)getHours() << " " << getMinutes() << "." << getSeconds();
		return ost.str();
	}
	string TimingSystem::getTimeStampHalf()
	{
		/*
			Returns a half timestamp of h,m,s.
		*/
		std::ostringstream ost;
		ost << (int)getHours() << " " << getMinutes() << "." << getSeconds();
		return ost.str();
	}
	void TimingSystem::updateTime()
	{
		/*
			Forces the boost time to update.
		*/
		currentTime = second_clock::local_time();
		currentTimeTM = to_tm(currentTime);
	}
	void TimingSystem::setTimerSeconds(int sec)
	{
		/*
			Sets the timer in seconds.
		*/
		updateTime();
		timer = currentTime + seconds(sec);
	}
	void TimingSystem::setTimerSeconds(double sec)
	{
		/*
			Sets the timer in seconds.
		*/
		updateTime();
		int wholeSeconds = (int)sec;
		double wholeMilliseconds = (sec - (int)sec) * 100;
		timer = currentTime + seconds((long)wholeSeconds) + milliseconds((long)wholeMilliseconds);
	}
	void TimingSystem::setTimerMilliseconds(int milsec)
	{
		/*
			Sets the timer in milliseconds.
		*/
		updateTime();
		timer = currentTime + milliseconds(milsec);
	}
	void TimingSystem::setTimerBoth(int min, int sec)
	{
		/*
			Sets the timer in both minutes and seconds.
		*/
		updateTime();
		timer = currentTime + minutes(min) + seconds(sec);
	}
	void TimingSystem::setTimerMSM(int min, int sec, int milsec)
	{
		/*
			Sets the timer in minutes, seconds, and milliseconds.
		*/
		updateTime();
		timer = currentTime + minutes(min) + seconds(sec) + milliseconds(milsec);
	}
	void TimingSystem::wait(int milSec)
	{
		/*
			Forces the current thread to pause for a certain amount of milSeconds.
		*/
		boost::this_thread::sleep(boost::posix_time::milliseconds(milSec));
	}
	bool TimingSystem::isTimerActive()
	{
		/*
			Checks if the timer is active or if the timer is already inactive.
		*/
		updateTime();
		if(timer > currentTime)
			return true;
		else
			return false;
	}

}
