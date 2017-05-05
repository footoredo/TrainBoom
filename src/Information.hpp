#ifndef TRAINBOOM_ROUTE_INFORMATION_HPP
#define TRAINBOOM_ROUTE_INFORMATION_HPP

#include <sstream>
#include <ctime>
#include "util.hpp"

namespace TrainBoom {

#define isStart 1
#define isEnd 2

class Information {
private:
    std::string stationId;
    unsigned distance;
    util::Datetime::Datetime arriveTime, leaveTime;
    unsigned flags;
    Id id;

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

    Information(Id stationId, unsigned distance,
        const util::Datetime::Datetime& arriveTime,
        const util::Datetime::Datetime& leaveTime,
        unsigned flags = 0
    ) : stationId(stationId), distance(distance),
        arriveTime(arriveTime), leaveTime(leaveTime), flags(flags),
        id("Information") {
            if (flags != 0)
                 throw wrong_information();
    }

    Information(Id stationId, unsigned distance,
        const util::Datetime::Datetime& tmpTime,
        unsigned flags = 0
    ) : stationId(stationId), distance(distance), flags(flags), id("Information") {
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

    /*Information(const Value& jsonValue)
        : stationId(jsonValue["data"]["stationId"].GetUint64()),
            distance(jsonValue["data"]["distance"].GetUint64()),
            flags(jsonValue["data"]["flags"].GetUint()) {
                assert(jsonValue["name"] == "information");
                if ((flags & isStart)) {
                    if (jsonValue["data"].HasMember("arriveTime"))
                        throw wrong_information();
                    leaveTime = Datetime::parse(jsonValue["data"])
                }
            }*/

    Information(const util::Json& json)
        : stationId(json["stationId"]),
            distance(json["distance"]),
            flags(json["flags"]), id("Information") {
                if (!(flags & isStart)) arriveTime = util::Datetime::Datetime::parse(json["arriveTime"]);
                if (!(flags & isEnd)) leaveTime = util::Datetime::Datetime::parse(json["leaveTime"]);
                // createTime = std::time(nullptr);
                // id = generateId("Information", createTime);
            }

    std::string getId() const {
        return id;
    }

    std::string getStationId() const {
        return stationId;
    }

    void setStationId(Id _stationId) {
        stationId = _stationId;
    }

    unsigned getDistance() const {
        return distance;
    }

    void setDistance(unsigned _distance) {
        if (flags == isStart)
            throw access_not_permitted();
        distance = _distance;
    }

    util::Datetime::Datetime getArriveTime() const {
        if (flags == isStart)
            throw access_not_permitted();
        return arriveTime;
    }

    void setArriveTime(util::Datetime::Datetime& _arriveTime) {
        if (flags == isStart)
            throw access_not_permitted();
        arriveTime = _arriveTime;
    }

    util::Datetime::Datetime getLeaveTime() const {
        if (flags == isEnd)
            throw access_not_permitted();
        return leaveTime;
    }

    void setLeaveTime(util::Datetime::Datetime& _leaveTime) {
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
        std::cout << "\tStation id: " << stationId << std::endl;
        std::cout << "\tDistance: " << distance << " km" << std::endl;
        if (!isStartStation()) std::cout << "\tArrive time: " << arriveTime << std::endl;
        if (!isEndStation()) std::cout << "\tLeave time: " << leaveTime << std::endl;
    }

    /*Document toJson() const {
        Document document;
        Document::AllocatorType& allocator = document.GetAllocator();
        Value& data = initiateDocument(document, "information");
        {
            data.AddMember("stationId", Value().SetUint64(stationId), allocator);
            data.AddMember("distance", Value().SetUint64(distance), allocator);
            if (!isStartStation()) data.AddMember("arriveTime", arriveTime.format(), allocator);
            if (!isEndStation()) data.AddMember("leaveTime", leaveTime.format(), allocator);
            data.AddMember("flags", Value().SetUint(flags), allocator);
        }
        return document;
    }*/

    Datetime getDate() const {
        if (isStartStation()) return leaveTime.clearTime();
        else return arriveTime.clearTime();
    }

    util::Json toJson() const {
        util::Json json("information", id);
        json["stationId"] = stationId;
        json["distance"] = distance;
        if (!isStartStation()) json["arriveTime"] = arriveTime.format();
        if (!isEndStation()) json["leaveTime"] = leaveTime.format();
        json["flags"] = flags;
        return json;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "stationId " << stationId << '\n'
            << "distance " << distance << '\n';
        if (!isStartStation()) ss << "arriveTime " << arriveTime.format() << '\n';
        if (!isEndStation()) ss << "leaveTime " << leaveTime.format() << '\n';
        ss << "flags " << flags << '\n';
        return ss.str();
    }
};

}

#endif
