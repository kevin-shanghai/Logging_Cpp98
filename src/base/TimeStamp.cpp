#include "Common/Logger/Log/include/base/TimeStamp.h"
#include "Common/Logger/Log/include/PlatformDefine.h"
#include <assert.h>
#include <sstream>
#include <time.h>
#include <iomanip>

namespace Logger
{
	static const int64_t kMicrosecondsPerSedonds = 1000 * 1000;
	static const int64_t kMicrosecondsPerMillisedonds = 1000;
	//Number of micro-seconds between the beginning of the Windows epoch (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
	static const int64_t kMicroseondsSinceEpochOnWindows = 116444736000000000ULL;

	TimeStamp::TimeStamp(int64_t iMicroSecondsSinceEpoch)
		: microSecondsSinceEpoch_(iMicroSecondsSinceEpoch)
	{

	}

	TimeStamp::TimeStamp(const TimeStamp& rhs)
	{
		this->microSecondsSinceEpoch_ = rhs.microSecondsSinceEpoch_;
	}

	TimeStamp TimeStamp::Now()
	{
		int64_t microSecondsSinceEpoch = 0;
#ifdef ON_WINDOWS
		FILETIME ft;
		LARGE_INTEGER li;
		GetSystemTimeAsFileTime(&ft);		  // accuracy 100 ns, 0.1 microseconds
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		microSecondsSinceEpoch = (li.QuadPart - kMicroseondsSinceEpochOnWindows) / 10;
#elif defined ON_LINUX
		struct timeval tv;
		::gettimeofday(&tv, NULL);
		microSecondsSinceEpoch = tv.tv_sec * kMicrosecondsPerSedonds + tv.tv_usec;

#else	//VXworks
        struct timespec tp;
        if  ( (clock_gettime(CLOCK_REALTIME, &tp))==0)
        {
            microSecondsSinceEpoch = tp.tv_sec * kMicrosecondsPerSedonds + tp.tv_nsec / 1000;
        }
#endif
		assert(microSecondsSinceEpoch > 0);
		return TimeStamp(microSecondsSinceEpoch);
	}


	std::string TimeStamp::ToCalenderTime()
	{
		std::stringstream ss;
		ss << std::setw(6)<< std::setfill('0') << (microSecondsSinceEpoch_ % kMicrosecondsPerSedonds);
		return GetCurrentLocalTime() + "." + ss.str();
       
	}

	double TimeStamp::DiffInSeconds(const TimeStamp& t1, const TimeStamp& t2)
	{
		double timeDiff = t1.microSecondsSinceEpoch_ - t2.microSecondsSinceEpoch_;
		return   timeDiff / kMicrosecondsPerSedonds;
	}

	double TimeStamp::DiffInMilliSedonds(const TimeStamp& t1, const TimeStamp& t2)
	{
		double timeDiff = t1.microSecondsSinceEpoch_ - t2.microSecondsSinceEpoch_;
		return timeDiff / kMicrosecondsPerMillisedonds;
	}

	int64_t TimeStamp::DiffInMicroSeconds(const TimeStamp& t1, const TimeStamp& t2)
	{
		int64_t timeDiff = t1.microSecondsSinceEpoch_ - t2.microSecondsSinceEpoch_;
		return timeDiff;
	}


	std::string TimeStamp::GetCurrentLocalTime()
	{
		time_t rawTime = time(&rawTime);
		tm* timeInfo = localtime(&rawTime);
		char buffer[25] = { '\0' };
		strftime(buffer, 25, "%Y-%m-%d %H-%M-%S", timeInfo);
		return buffer;
	}
}
