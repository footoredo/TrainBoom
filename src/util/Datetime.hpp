#ifndef TRAINBOOM_UTIL_DATETIME_HPP
#define TRAINBOOM_UTIL_DATETIME_HPP

#include "exception.hpp"
// #include "util.hpp"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ctime>

namespace trainBoom {

namespace util {

namespace date_time {

    class time_out_of_range : public exception {
    public:
    	time_out_of_range() : exception(
    		"time_out_of_range",
    		"Your time is out of range!!!") {}
    };

    class time_format_wrong : public exception {
    public:
    	time_format_wrong() : exception(
    		"time_format_wrong",
    		"Your time format is wrong!!!") {}
    };

    class not_initiated : public exception {
    public:
    	not_initiated() : exception(
    		"not_initiated",
    		"Your time is not initiated!!!") {}
    };

    class duration_is_negative : public exception {
    public:
    	duration_is_negative() : exception(
    		"duration_is_negative",
    		"Your duration is negative!!!") {}
    };

    typedef unsigned short Date_t;
    typedef short signed_Date_t;

    signed_Date_t getNum(const std::string& s, int st, int en)
    {
        signed_Date_t num = 0;
        for(int i = st; i <= en; i++)
        {
            if (s[i] < '0' || s[i] > '9') continue;
            num = num * 10 + s[i] - '0';
            if (num > 9999) throw time_out_of_range();
        }
        return num;
    }

    class Duration {
    private:
        Date_t hour, minute;
    public:
        Duration(signed_Date_t _hour = 0, signed_Date_t _minute = 0) {
                minute = (_minute % 60 + 60) % 60; _hour += (_minute - minute) / 60;
                hour = _hour;
        }

        //Duration(const std::string& fmttime)

        std::string format() const noexcept {
            std::stringstream buffer;
            /*
            buffer << day << " day" << (day > 1 ? "s" : "" ) << " "
                << hour << " hour" << (hour > 1 ? "s" : "" ) << " "
                << minute << " minute" << (minute > 1 ? "s" : "" );
*/
            buffer << hour << ':'
                << std::setw(2) << std::setfill('0')
                << minute;
            return buffer.str();
        }

        operator std::string() const noexcept {
            return format();
        }

        Duration operator+(const Duration& duration) const {
            return Duration (
                hour + duration.hour,
                minute + duration.minute
            );
        }

        Duration operator-(const Duration& duration) const {
            return Duration (
                signed_Date_t(hour) - duration.hour,
                signed_Date_t(minute) - duration.minute
            );

        }

        Duration setToDay() const {
            return Duration(hour / 24 * 24, 0);
        }

        Date_t countDay() const {
            return hour / 24;
        }

        Date_t getHour() const noexcept {
            return hour;
        }

        Date_t getMinute() const noexcept {
            return minute;
        }

        friend std::ostream& operator<<(std::ostream& os, const Duration& duration) {
            os << duration.format();
            return os;
        }

        Duration(std::string fmt) {
            Duration tmp = parse(fmt);
            hour = tmp.hour;
            minute = tmp.minute;
        }

        static Duration parse(const std::string& fmttime) // YYYY/MM/DD hh:mm
        {
        	int len = fmttime.size();
        	int colon = 0;

        	for (int i = 0; i < len; i++)
        	{
	        	if (fmttime[i] == ':')
				{
					if (!colon) colon = i;
					else throw time_format_wrong();
				}
	        }
       		if (!colon) throw time_format_wrong();
        	signed_Date_t hour = getNum(fmttime, 0, colon - 1), minute = getNum(fmttime, colon + 1, len - 1);
            return Duration(hour, minute);
        }
    };

    class Datetime {
    private:
        Date_t year, month, day, hour, minute;
        bool initiated, dayOnly;
        static bool isLeapYear(Date_t year) {
            if (year % 100 == 0)
                return year % 400 == 0;
            else
                return year % 4 == 0;
        }
        static Date_t getDayOfMonth(Date_t year, Date_t month) {
            if (month < 1 || month > 12)
                throw time_out_of_range();
            if (month == 2)
                return 28 + isLeapYear(year);
            else if (month == 1 || month == 3 || month == 5 || month == 7 ||
                    month == 8 || month == 10 || month == 12)
                return 31;
            else
                return 30;
        }
    public:
        Datetime() noexcept: initiated(false) {}
        Datetime(signed_Date_t _year, signed_Date_t _month, signed_Date_t _day,
            signed_Date_t _hour, signed_Date_t _minute): initiated(true), dayOnly(false) {
                if (_year < 0 || _year > 9999 || _month < 1 || _month > 12) {
                    // std::cout << "!!!" << std::endl;
                    throw time_out_of_range();
                }
                minute = (_minute % 60 + 60) % 60; _hour += (_minute - minute) / 60;
                hour = (_hour % 24 + 24) % 24; _day += (_hour - hour) / 24;
                while (_day > getDayOfMonth(_year, _month)) {
                    _day -= getDayOfMonth(_year, _month);
                    ++ _month;
                    if (_month > 12) {
                        _month -= 12;
                        ++ _year;
                    }
                }

                while (_day < 1) {
                    -- _month;
                    if (_month < 1) {
                        _month += 12;
                        -- _year;
                        if (_year < 0)
                            throw time_out_of_range();
                    }
                    _day += getDayOfMonth(_year, _month);
                }

                day = _day;
                month = _month;
                year = _year;
        }
        Datetime(signed_Date_t _year, signed_Date_t _month, signed_Date_t _day)
            : initiated(true), dayOnly(true) {
                if (_year < 0 || _year > 9999 || _month < 1 || _month > 12) {
                    // std::cout << "!!!" << std::endl;
                    throw time_out_of_range();
                }
                minute = 0;
                hour = 0;
                while (_day > getDayOfMonth(_year, _month)) {
                    _day -= getDayOfMonth(_year, _month);
                    ++ _month;
                    if (_month > 12) {
                        _month -= 12;
                        ++ _year;
                    }
                }

                while (_day < 1) {
                    -- _month;
                    if (_month < 1) {
                        _month += 12;
                        -- _year;
                        if (_year < 0)
                            throw time_out_of_range();
                    }
                    _day += getDayOfMonth(_year, _month);
                }

                day = _day;
                month = _month;
                year = _year;
        }

        static Datetime parse(const std::string& fmttime) // YYYY/MM/DD hh:mm
        {
        	int len = fmttime.size();
        	int _1 = 0, _2 = 0, colon = 0, space = 0;

        	for (int i = 0; i < len; i++)
        	{
	        	if (fmttime[i] == ':')
				{
					if (!_1 || !_2 || !space) throw time_format_wrong();
					else if (!colon) colon = i;
					else throw time_format_wrong();
				}
				else if (fmttime[i] == ' ')
				{
					if (!_1 || !_2) throw time_format_wrong();
					else if (!space) space = i;
					else throw time_format_wrong();
				}
	        	else if (fmttime[i] == '/' || fmttime[i] == '-')
	        	{
	        		if (!_1) _1 = i;
	        		else _2 = i;
	        		// else throw time_format_wrong();
	        	}
	        }
       		if (!_1 || !_2) throw time_format_wrong();
            if (!space) space = fmttime.size();
        	signed_Date_t year = getNum(fmttime, 0, _1 - 1), month = getNum(fmttime, _1 + 1, _2 - 1), day = getNum(fmttime, _2 + 1, space - 1);
            if (!colon) {
                return Datetime(year, month, day);
            }
            else {
                signed_Date_t hour = getNum(fmttime, space + 1, colon - 1), minute = getNum(fmttime, colon + 1, len - 1);
                return Datetime(year, month, day, hour, minute);
            }
        }

        Datetime(std::string fmt) {
            Datetime tmp = parse(fmt);
            year = tmp.year;
            month = tmp.month;
            day = tmp.day;
            hour = tmp.hour;
            minute = tmp.minute;
            initiated = tmp.initiated;
            dayOnly = tmp.dayOnly;
        }

        static Datetime now() {
            time_t tn = std::time(nullptr);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&tn), "%Y/%m/%d %R");
            return parse(ss.str());
        }

        std::string format() const {
            if (!initiated)
                throw not_initiated();
            std::stringstream buffer;
            buffer << year << '/' << month << '/' << day;
            if (!dayOnly)
            buffer << ' '
                << std::setw(2) << std::setfill('0')
                << hour << ':'
                << std::setw(2) << std::setfill('0')
                << minute;

            return buffer.str();
        }

        operator std::string() const {
            return format();
        }

        Datetime operator+(const Duration& duration) const {
            if (!initiated)
                throw not_initiated();
            Datetime tmp (
                year, month, day,
                hour + duration.getHour(),
                minute + duration.getMinute()
            );
            if (dayOnly) {
                tmp = tmp.clearTime();
            }
            return tmp;
        }

        Datetime operator-(const Duration& duration) const {
            if (!initiated)
                throw not_initiated();
            return Datetime (
                year, month, day,
                signed_Date_t(hour) - duration.getHour(),
                signed_Date_t(minute) - duration.getMinute()
            );
        }

        Duration operator-(Datetime datetime) const {
            if (!initiated || !datetime.initiated)
                throw not_initiated();
            if (year < datetime.year ||
                (year == datetime.year && month < datetime.month) ||
                (year == datetime.year && month == datetime.month && day < datetime.day)) {
                    throw duration_is_negative();
                }
            int dayCount = 0;
            while (datetime.year != year ||
                datetime.month != month ||
                datetime.day != day) {
                    // std::cout << datetime << std::endl;
                    datetime = datetime.incDay();
                    ++ dayCount;
                }
            return Duration (
                dayCount * 24 +
                signed_Date_t(hour) - datetime.getHour(),
                signed_Date_t(minute) - datetime.getMinute()
            );
        }

        Datetime incDay() const {
            return this->operator+(Duration(24, 0));
        }

        Date_t getYear() const {
            if (!initiated)
                throw not_initiated();
            return year;
        }

        Date_t getMonth() const {
            if (!initiated)
                throw not_initiated();
            return month;
        }

        Date_t getDay() const {
            if (!initiated)
                throw not_initiated();
            return day;
        }

        Date_t getHour() const {
            if (!initiated)
                throw not_initiated();
            return hour;
        }

        Date_t getMinute() const {
            if (!initiated)
                throw not_initiated();
            return minute;
        }

        Datetime clearTime() const {
            if (!initiated)
                throw not_initiated();
            return Datetime(year, month, day);
        }

        bool operator<(const Datetime& other) const {
            if (!initiated || !other.initiated)
                throw not_initiated();
            if (year == other.year)
                if (month == other.month)
                    if (day == other.day)
                        if (hour == other.hour)
                            return minute < other.minute;
                        else
                            return hour < other.hour;
                    else
                        return day < other.day;
                else
                    return month < other.month;
            else
                return year < other.year;
        }

        bool operator==(const Datetime& other) const {
            if (!initiated || !other.initiated)
                throw not_initiated();
            if (dayOnly != other.dayOnly) return false;
            if (!( year == other.year &&
                    month == other.month &&
                    day == other.day ))
                return false;
            if (!dayOnly)
                if (!( hour == other.hour &&
                    minute == other.minute))
                    return false;
            return true;
        }

        friend std::ostream& operator<<(std::ostream& os, const Datetime& datetime) {
            if (!datetime.initiated)
                throw not_initiated();
            os << datetime.format();
            return os;
        }
    };

}   // Datetime

}   // util

using util::date_time::Duration;
using util::date_time::Datetime;

}   // TrainBoom

#endif
