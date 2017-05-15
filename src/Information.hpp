#ifndef TRAINBOOM_ROUTE_INFORMATION_HPP
#define TRAINBOOM_ROUTE_INFORMATION_HPP

#include <sstream>
#include <ctime>
#include "Id.hpp"
#include "util/Datetime.hpp"
#include "DataManager.hpp"
#include "util/Json.hpp"
#include "util/stupid_ptr.hpp"

namespace trainBoom {

#define isStart 1
#define isEnd 2

class Information {
private:
    std::string stationName;
    unsigned distance;
    Duration arriveTime, leaveTime;
    unsigned flags;
    // std::string id;

public:
    class wrong_information : public exception {
    public:
    	wrong_information() : exception(
    		"wrong_information",
    		"Your information is wrong!!!") {}
    };

    class access_not_permitted : public exception {
    public:
    	access_not_permitted() : exception(
    		"access_not_permitted",
    		"Your access is not permitted ( Did you access arriveTime of a start station or such ? ) !!!") {}
    };

    Information(std::string stationName, unsigned distance,
        const Duration& arriveTime,
        const Duration& leaveTime,
        unsigned flags = 0
    ) : stationName(stationName), distance(distance),
        arriveTime(arriveTime), leaveTime(leaveTime), flags(flags) {
            if (flags != 0)
                 throw wrong_information();
    }

    Information(std::string stationName, unsigned distance,
        const Duration& tmpTime,
        unsigned flags = 0
    ) : stationName(stationName), distance(distance), flags(flags) {
            if (flags == 0)
                throw wrong_information();
            else if ((flags & isStart) && (flags & isEnd))
                throw wrong_information();
            else if (flags == isStart) {
                if (distance != 0)
                    throw wrong_information();
                leaveTime = tmpTime;
                // std::cout << leaveTime << std::endl;
            }
            else if (flags == isEnd)
                arriveTime = tmpTime;
            else
                throw wrong_information();
    }

    Information(const util::Json& json)
        : stationName(json["stationName"].as<std::string>()),
            distance(json["distance"].as<unsigned>()),
            flags(json["flags"].as<unsigned>()) {
                // if (json.getId() != "") id = json.getId();
                // else id = Id("Information");
                if (!(flags & isStart)) arriveTime = Duration::parse(json["arriveTime"]);
                if (!(flags & isEnd)) leaveTime = Duration::parse(json["leaveTime"]);
                // createTime = std::time(nullptr);
                // id = generateId("Information", createTime);
            }

    void shift(Duration dayShift) {
        arriveTime = arriveTime - dayShift;
        leaveTime = leaveTime - dayShift;
    }

    std::string getStationName() const {
        return stationName;
    }

    void setStationName(Id _stationName) {
        stationName = _stationName;
    }

    unsigned getDistance() const {
        return distance;
    }

    void setDistance(unsigned _distance) {
        if (flags == isStart)
            throw access_not_permitted();
        distance = _distance;
    }

    Duration getArriveTime() const {
        if (flags == isStart)
            throw access_not_permitted();
        return arriveTime;
    }

    void setArriveTime(Duration& _arriveTime) {
        if (flags == isStart)
            throw access_not_permitted();
        arriveTime = _arriveTime;
    }

    Duration getLeaveTime() const {
        if (flags == isEnd)
            throw access_not_permitted();
        return leaveTime;
    }

    void setLeaveTime(Duration& _leaveTime) {
        if (flags == isEnd)
            throw access_not_permitted();
        leaveTime = _leaveTime;
    }

    bool isStartStation() const {
        return flags == isStart;
    }

    bool isEndStation() const {
        return flags == isEnd;
    }

    void display() const {
        std::cout << "\tStation Name: " << stationName << std::endl;
        std::cout << "\tDistance: " << distance << " km" << std::endl;
        if (!isStartStation()) std::cout << "\tArrive time: " << arriveTime << std::endl;
        if (!isEndStation()) std::cout << "\tLeave time: " << leaveTime << std::endl;
    }

    // Duration getDate() const {
    //     if (isStartStation()) return leaveTime.clearTime();
    //     else return arriveTime.clearTime();
    // }

    util::Json toJson() const {
        util::Json json;
        json["stationName"] = stationName;
        json["distance"] = distance;
        if (!isStartStation()) json["arriveTime"] = arriveTime.format();
        if (!isEndStation()) json["leaveTime"] = leaveTime.format();
        json["flags"] = flags;
        return json;
    }

    util::Json toJson(Datetime date) const {
	    date = date.enableTime();
        util::Json json;
        json["stationName"] = stationName;
        json["distance"] = distance;
        if (!isStartStation()) json["arriveTime"] = (date + arriveTime).format();
        if (!isEndStation()) json["leaveTime"] = (date + leaveTime).format();
        json["flags"] = flags;
        return json;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "stationName " << stationName << '\n'
            << "distance " << distance << '\n';
        if (!isStartStation()) ss << "arriveTime " << arriveTime.format() << '\n';
        if (!isEndStation()) ss << "leaveTime " << leaveTime.format() << '\n';
        ss << "flags " << flags << '\n';
        return ss.str();
    }
};

}

#endif
