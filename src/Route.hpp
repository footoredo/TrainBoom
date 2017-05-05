#ifndef TRAINBOOM_ROUTE_HPP
#define TRAINBOOM_ROUTE_HPP

#include "util.hpp"
#include "Order.hpp"
#include "Segment.hpp"
#include "Information.hpp"
#include <iostream>
#include <sstream>

namespace TrainBoom {

typedef util::IntervalManip<
    Segment,
    TicketDelta,
    Segment::Modifier,
    Segment::MergerT,
    Segment::MergerM
> SegmentsInvervalManip;

class Information;

class Route {
private:
    std::string name;

    unsigned n;

    util::stupid_array<util::stupid_ptr<Information>> informations;

    util::stupid_array<util::stupid_ptr<Segment>> segments;

    util::stupid_ptr<SegmentsInvervalManip> segmentsIntervalManip;
    util::map<std::string, unsigned> stationsMap;

    Id id;

    util::pair<unsigned, unsigned> getInterval(const std::string& startStation, const std::string& endStation) {
        const auto& iterStart = stationsMap.find(startStation),
            iterEnd = stationsMap.find(endStation);
        if (iterStart == stationsMap.end() ||
            iterEnd == stationsMap.end()) {
                throw station_not_found();
            }
        unsigned start = iterStart->second, end = iterEnd->second;
        if (end <= start) {
            throw interval_invalid();
        }

        return util::make_pair(start, end - 1); // Attenion here !!
    }

public:
    class station_number_too_small : public exception {
    public:
    	station_number_too_small() : exception(
    		"station_number_too_small",
    		"Your station number is too small!!!") {}
    };

    class information_missing : public exception {
    public:
    	information_missing(const std::string& info) : exception(
    		"information_missin",
    		"Your " + info + " is missing!!!") {}
    };

    class station_number_not_consistent : public exception {
    public:
    	station_number_not_consistent() : exception(
    		"station_number_not_consistent",
    		"Your station number is not consistent among all information!!!") {}
    };

    class station_not_found : public exception {
    public:
    	station_not_found() : exception(
    		"station_not_found",
    		"Your station is not found!!!") {}
    };

    class index_out_of_range : public exception {
    public:
    	index_out_of_range() : exception(
    		"index_out_of_range",
    		"Your index is out of range!!!") {}
    };

    class interval_invalid : public exception {
    public:
    	interval_invalid() : exception(
    		"interval_invalid",
    		"Your requested interval is not valid!!!") {}
    };

    class not_enough_tickets_left: public exception {
    public:
    	not_enough_tickets_left() : exception(
    		"not_enough_tickets_left",
    		"Tickets left are not enough for you to book!!!") {}
    };

    explicit Route(): id("Route") {}

    /*Route(Id id, unsigned n,
        const util::stupid_array<Id>& stations,
        const util::stupid_array<unsigned>& distance,
        const util::stupid_array<util::Datetime::Datetime>& arriveTime,
        const util::stupid_array<util::Datetime::Datetime>& leaveTime,
        const util::stupid_array<Segment>& segments
    ): id(id), n(n), stations(stations), distance(distance),
        arriveTime(arriveTime), leaveTime(leaveTime), segments(segments) {
            if (n < 2) {
                throw station_number_too_small();
            }
            if (stations.size() != n || distance.size() != n - 1 ||
                arriveTime.size() != n - 1 || leaveTime.size() != n - 1 ||
                segments.size() != n - 1) {
                    throw station_number_not_consistent();
                }

            for (unsigned i = 0; i < n; ++ i)
                stationsMap[stations[i]] = i;

            segmentsIntervalManip = new SegmentsInvervalManip(
                segments, n - 1
            );
        }*/

    Route(std::string name, unsigned n,
        const util::stupid_array<util::stupid_ptr<Information>>& informations,
        const util::stupid_array<util::stupid_ptr<Segment>>& segments
    ): name(name), n(n), informations(informations), segments(segments), id("Route") {
            if (n < 2) {
                throw station_number_too_small();
            }
            if (informations.size() != n || segments.size() != n - 1) {
                    throw station_number_not_consistent();
                }
            for (unsigned i = 0; i < n; ++ i)
                stationsMap[informations[i]->getStationId()] = i;
            segmentsIntervalManip = new SegmentsInvervalManip(
                segments, n - 1
            );
        }

    Route(const Json& json): id("Route") {
        if (!json.HasMember("n")) throw information_missing("n");
        if (!json.HasMember("name")) throw information_missing("name");
        if (!json.HasMember("informations")) throw information_missing("informations");
        if (!json.HasMember("segments")) throw information_missing("segments");

        n = json["n"].as<unsigned>();
        if (n < 2) {
            throw station_number_too_small();
        }
        name = json["name"].as<std::string>();
        if (json["informations"].Size() != n || json["segments"].Size() != n - 1) {
            throw station_number_not_consistent();
        }
        informations = stupid_array<stupid_ptr<Information>>(new stupid_ptr<Information>[n], n);
        segments = stupid_array<stupid_ptr<Segment>>(new stupid_ptr<Segment>[n - 1], n - 1);
        for (unsigned i = 0; i < n; ++ i) {
            informations[i] = make_stupid<Information>(json["informations"][i]);
            if (i < n - 1) {
                segments[i] = make_stupid<Segment>(json["segments"][i]);
            }
        }

        for (unsigned i = 0; i < n; ++ i)
            stationsMap[informations[i]->getStationId()] = i;

        segmentsIntervalManip = new SegmentsInvervalManip(segments, n - 1);
    }

    /*void rebuild(unsigned _n,
        const util::stupid_array<Id>& _stations,
        const util::stupid_array<unsigned>& _distance,
        const util::stupid_array<util::Datetime::Datetime>& _arriveTime,
        const util::stupid_array<util::Datetime::Datetime>& _leaveTime,
        const util::stupid_array<Segment>& _segments) {
            if (_n < 2) {
                throw station_number_too_small();
            }
            if (_stations.size() != n || _distance.size() != n - 1 ||
                _arriveTime.size() != n - 1 || _leaveTime.size() != n - 1 ||
                _segments.size() != n - 1) {
                    throw station_number_not_consistent();
                }

            n = _n;
            stations = _stations;
            distance = _distance;
            arriveTime = _arriveTime;
            leaveTime = _leaveTime;
            segments = _segments;

            stationsMap.clear();
            for (unsigned i = 0; i < n; ++ i)
                stationsMap[stations[i]] = i;

            segmentsIntervalManip = new SegmentsInvervalManip(
                segments, n - 1
            );
        }*/

    std::string getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    unsigned size() const {
        return n;
    }

    Segment queryTickets(const std::string& startStation, const std::string& endStation) {
        auto interval = getInterval(startStation, endStation);
        return segmentsIntervalManip->query(interval.first, interval.second);
    }

    void modifyTickets(const std::string& startStation, const std::string& endStation, const TicketDelta& deltas) {
        auto interval = getInterval(startStation, endStation);
        segmentsIntervalManip->modify(interval.first, interval.second, deltas);
    }

    Order bookTickets(const std::string& startStationId, const std::string& endStationId, const std::string& ticketType, unsigned ticketNumber) {
        auto interval = getInterval(startStationId, endStationId);
        Segment segment = segmentsIntervalManip->query(interval.first, interval.second);
        if (segment.ticket(ticketType).number < ticketNumber)
            throw not_enough_tickets_left();
        Order order(id, startStationId, endStationId, ticketType, segment.ticket(ticketType).price * ticketNumber, ticketNumber);
        TicketDelta ticketDelta;
        ticketDelta[ticketType] = - (int)ticketNumber;
        segmentsIntervalManip->modify(interval.first, interval.second, ticketDelta);
        return order;
    }

    Information& information(unsigned pos) {
        if (pos >= n) {
            throw index_out_of_range();
        }
        return *(informations[pos]);
    }

    Information information(unsigned pos) const {
        if (pos >= n) {
            throw index_out_of_range();
        }
        return *(informations[pos]);
    }

    util::Datetime::Datetime getRunningDay() const noexcept {
        return informations[0]->getLeaveTime().clearTime();
    }

    void display() {
        segmentsIntervalManip->forceApply();
        std::cout << "\n----\n" << std::endl;
        std::cout << "id: " << id << std::endl;
        std::cout << "nStation: " << n << std::endl;
        for (unsigned i = 0; i < n; ++ i) {
            std::cout << "\nStation #" << i << ": " << std::endl;
            informations[i]->display();
            if (i < n - 1) {
                std::cout << "\tTicket information: \n" << std::endl;
                segments[i]->display();
                // std::cout << "#####################" << std::endl;
            }
        }
        std::cout << "\n---\n" << std::endl;
    }

    util::Json toJson() {
        segmentsIntervalManip->forceApply();

        util::Json json("route", id);

        json["name"] = name;
        json["n"] = n;
        json["informations"].SetArray();
        json["segments"].SetArray();
        // json["stationsMap"].SetObject();

        for (unsigned int i = 0; i < n; ++ i) {
            json["informations"].PushBack(informations[i]->toJson());
            // std::cout << "!!asdasd!" << std::endl;
            if (i + 1 < n)
                json["segments"].PushBack(segments[i]->toJson());
            // std::cout << "!!asdasd!" << i << std::endl;
        }

        /* for (const auto& item: stationsMap) {
            json["stationsMap"][item.first] = item.second;
        } */

        return json;
    }

    std::string toString() {
        segmentsIntervalManip->forceApply();
        std::stringstream ss;
        ss << "name " << name << '\n';
        ss << "n " << n << '\n';
        ss << "informations " << n << '\n';
        for (unsigned i = 0; i < n; ++ i)
            ss << "information " << informations[i]->getId() << '\n';
        ss << "segments " << n - 1 << '\n';
        for (unsigned i = 0; i < n - 1; ++ i)
            ss << "segment " << segments[i]->getId() << '\n';
        return ss.str();
    }
};

}   // TrainBoom

#endif
