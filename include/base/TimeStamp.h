#ifndef TIME_STAMP_H__
#define TIME_STAMP_H__
#include <string>
#include <stdint.h>

namespace Logger
{
	class TimeStamp
	{
	public:
		explicit TimeStamp(int64_t iMicroSecondsSinceEpoch);

		TimeStamp(const TimeStamp& rhs);

		static TimeStamp Now();

		static std::string GetCurrentLocalTime();

		static double DiffInSeconds(const TimeStamp& t1, const TimeStamp& t2);

		static double DiffInMilliSedonds(const TimeStamp& t1, const TimeStamp& t2);

		static int64_t DiffInMicroSeconds(const TimeStamp& t1, const TimeStamp& t2);

		std::string ToCalenderTime();

		

	private:
		 int64_t microSecondsSinceEpoch_;

	};
}



#endif