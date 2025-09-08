export module Time;
#include "Core.h"
import std;

namespace NSL
{
	struct TimePoint;
	void _GetYear(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;
	void _GetMonth(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;
	void _GetDay(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;
	void _GetHour(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;
	void _GetMinute(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;
	void _GetSecond(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT;

	export struct NSL_API TimePoint final
	{
		TimePoint() NSL_NOEXCEPT
			: year(1970)
			, month(1)
			, day(1)
			, hour(0)
			, minute(0)
			, second(0)
			, sinceEpoch(0)
		{
		}

		TimePoint operator-(const TimePoint& other) NSL_NOEXCEPT
		{
			TimePoint result;
			result.sinceEpoch = sinceEpoch - other.sinceEpoch;
			
			_GetYear(result, result.sinceEpoch);
			_GetMonth(result, result.sinceEpoch);
			_GetDay(result, result.sinceEpoch);
			_GetHour(result, result.sinceEpoch);
			_GetMinute(result, result.sinceEpoch);
			_GetSecond(result, result.sinceEpoch);

			return result;
		}

		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		long long sinceEpoch;
	};

	bool _IsLeapYear(int year) NSL_NOEXCEPT
	{
		if (!(year % 4))
		{
			if (!(year % 100))
			{
				if (!(year % 400))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	void _GetYear(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		static const int SECONDS_IN_YEAR = 31536000;
		static const int SECONDS_IN_LEAP_YEAR = 31622400;

		while (secondsSinceEpoch / SECONDS_IN_YEAR > 0 || secondsSinceEpoch / SECONDS_IN_LEAP_YEAR > 0)
		{
			if (_IsLeapYear(timePoint.year + 1))
			{
				secondsSinceEpoch -= SECONDS_IN_LEAP_YEAR;
			}
			else
			{
				secondsSinceEpoch -= SECONDS_IN_YEAR;
			}

			timePoint.year += 1;
		}
	}
	void _GetMonth(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		static const int SECONDS_IN_FEBRUARY = 2419200;
		static const int SECONDS_IN_FEBRUARY_LEAP = 2505600;
		static int SECONDS_IN_EACH_MONTH[12] = { 2678400, 0, 2678400, 2592000, 2678400,
			2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400 };

		if (_IsLeapYear(timePoint.year))
		{
			SECONDS_IN_EACH_MONTH[1] = SECONDS_IN_FEBRUARY_LEAP;
		}
		else
		{
			SECONDS_IN_EACH_MONTH[1] = SECONDS_IN_FEBRUARY;
		}

		while (secondsSinceEpoch >= SECONDS_IN_EACH_MONTH[timePoint.month - 1])
		{
			secondsSinceEpoch -= SECONDS_IN_EACH_MONTH[timePoint.month - 1];
			++timePoint.month;
		}
	}
	void _GetDay(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		static const int SECONDS_IN_DAY = 86400;
		while (secondsSinceEpoch >= SECONDS_IN_DAY)
		{
			secondsSinceEpoch -= SECONDS_IN_DAY;
			++timePoint.day;
		}
	}
	void _GetHour(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		static const int SECONDS_IN_HOUR = 3600;
		while (secondsSinceEpoch >= SECONDS_IN_HOUR)
		{
			secondsSinceEpoch -= SECONDS_IN_HOUR;
			++timePoint.hour;
		}
	}
	void _GetMinute(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		static const int SECONDS_IN_MINUTE = 60;
		while (secondsSinceEpoch >= SECONDS_IN_MINUTE)
		{
			secondsSinceEpoch -= SECONDS_IN_MINUTE;
			++timePoint.minute;
		}
	}
	void _GetSecond(TimePoint& timePoint, long long& secondsSinceEpoch) NSL_NOEXCEPT
	{
		timePoint.second = static_cast<int>(secondsSinceEpoch);
	}

	export TimePoint NSL_API GetTime() NSL_NOEXCEPT
	{
		TimePoint timePoint;

		auto now = std::chrono::system_clock::now();
		const auto epoch = now.time_since_epoch();
		long long secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(epoch).count();
		timePoint.sinceEpoch = epoch.count();

		_GetYear(timePoint, secondsSinceEpoch);
		_GetMonth(timePoint, secondsSinceEpoch);
		_GetDay(timePoint, secondsSinceEpoch);
		_GetHour(timePoint, secondsSinceEpoch);
		_GetMinute(timePoint, secondsSinceEpoch);
		_GetSecond(timePoint, secondsSinceEpoch);

		return timePoint;
	}
}