#ifndef TRAINBOOM_UTIL_DATETIME_HPP
#define TRAINBOOM_UTIL_DATETIME_HPP

#include "exception.hpp"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

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

    class Duration;

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

        // Duration(const std::string& fmttime);

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
    };

    class Datetime {
    private:
        Date_t year, month, day, hour, minute;
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
        Datetime(signed_Date_t _year, signed_Date_t _month, signed_Date_t _day,
            signed_Date_t _hour = 0, signed_Date_t _minute = 0) {
                if (_year < 0 || _month < 1 || _month > 12) {
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
        Datetime(const std::string& fmttime);

        std::string format() const noexcept {
            std::stringstream buffer;
            buffer << year << '/' << month << '/' << day << ' '
                << std::setw(2) << std::setfill('0')
                << hour << ':' << minute;

            return buffer.str();
        }

        operator std::string() const noexcept {
            return format();
        }

        Datetime operator+(const Duration& duration) const {
            return Datetime (
                year, month,
                day + duration.getDay(),
                hour + duration.getHour(),
                minute + duration.getMinute()
            );
        }

        Datetime operator-(const Duration& duration) const {
            return Datetime (
                year, month,
                signed_Date_t(day) - duration.getDay(),
                signed_Date_t(hour) - duration.getHour(),
                signed_Date_t(minute) - duration.getMinute()
            );
        }

        Date_t getYear() const noexcept {
            return year;
        }

        Date_t getMonth() const noexcept {
            return month;
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
    };

}

}

#endif
