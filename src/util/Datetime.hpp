#ifndef TRAINBOOM_UTIL_DATETIME_HPP
#define TRAINBOOM_UTIL_DATETIME_HPP

#include "exception.hpp"
#include "util.hpp"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace TrainBoom {

namespace util {

namespace Datetime {

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

    typedef unsigned short Date_t;
    typedef short signed_Date_t;

    class Duration {
    private:
        Date_t day, hour, minute;
    public:
        Duration(signed_Date_t _day = 0, signed_Date_t _hour = 0, signed_Date_t _minute = 0) {
                minute = (_minute % 60 + 60) % 60; _hour += (_minute - minute) / 60;
                hour = (_hour % 24 + 24) % 24; _day += (_hour - hour) / 24;
            if (_day < 0) {
                // std::cout << "!!!" << std::endl;
                throw time_out_of_range();
            }
            else {
                day = _day;
            }
        }

        //Duration(const std::string& fmttime)

        std::string format() const noexcept {
            std::stringstream buffer;
            buffer << day << " day" << (day > 1 ? "s" : "" ) << " "
                << hour << " hour" << (hour > 1 ? "s" : "" ) << " "
                << minute << " minute" << (minute > 1 ? "s" : "" );

            return buffer.str();
        }

        operator std::string() const noexcept {
            return format();
        }

        Duration operator+(const Duration& duration) const {
            return Duration (
                day + duration.day,
                hour + duration.hour,
                minute + duration.minute
            );
        }

        Duration operator-(const Duration& duration) const {
            return Duration (
                signed_Date_t(day) - duration.day,
                signed_Date_t(hour) - duration.hour,
                signed_Date_t(minute) - duration.minute
            );
        }

        Date_t getDay() const noexcept {
            return day;
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

        static signed_Date_t getNum(const std::string& s, int st, int en)
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
	        	else if (fmttime[i] == '/')
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
            return Datetime (
                year, month,
                day + duration.getDay(),
                hour + duration.getHour(),
                minute + duration.getMinute()
            );
        }

        Datetime operator-(const Duration& duration) const {
            if (!initiated)
                throw not_initiated();
            return Datetime (
                year, month,
                signed_Date_t(day) - duration.getDay(),
                signed_Date_t(hour) - duration.getHour(),
                signed_Date_t(minute) - duration.getMinute()
            );
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

using util::Datetime::Datetime;

}   // TrainBoom

#endif
