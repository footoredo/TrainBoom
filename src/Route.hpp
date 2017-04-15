#ifndef TRAINBOOM_ROUTE_HPP
#define TRAINBOOM_ROUTE_HPP

#include "util.hpp"
#include "Segment.hpp"
#include <iostream>

namespace TrainBoom {

typedef util::IntervalManip<
    Segment,
    TicketDelta,
    Segment::Modifier,
    Segment::MergerT,
    Segment::MergerM
> SegmentsInvervalManip;

class Route {
private:
    id_t id;

    size_t n;
    util::stupid_array<id_t> stations;
    util::stupid_array<size_t> distance;
    util::stupid_array<util::Datetime::Datetime> arriveTime;
    util::stupid_array<util::Datetime::Datetime> leaveTime;

    util::stupid_array<Segment> segments;

    util::stupid_ptr<SegmentsInvervalManip> segmentsIntervalManip;
    util::map<id_t, size_t> stationsMap;

    util::pair<size_t, size_t> getInterval(id_t startStation, id_t endStation) {
        const auto& iterStart = stationsMap.find(startStation),
            iterEnd = stationsMap.find(endStation);
        if (iterStart == stationsMap.end() ||
            iterEnd == stationsMap.end()) {
                throw station_not_found();
            }
        size_t start = iterStart->second, end = iterEnd->second;
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

    class interval_invalid : public exception {
    public:
    	interval_invalid() : exception(
    		"interval_invalid",
    		"Your requested interval is not valid!!!") {}
    };

    explicit Route(id_t id): id(id) {}

    Route(id_t id, size_t n,
        const util::stupid_array<id_t>& stations,
        const util::stupid_array<size_t>& distance,
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

            for (size_t i = 0; i < n; ++ i)
                stationsMap[stations[i]] = i;

            segmentsIntervalManip = new SegmentsInvervalManip(
                segments, n - 1
            );
        }

    void rebuild(size_t _n,
        const util::stupid_array<id_t>& _stations,
        const util::stupid_array<size_t>& _distance,
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
            for (size_t i = 0; i < n; ++ i)
                stationsMap[stations[i]] = i;

            segmentsIntervalManip = new SegmentsInvervalManip(
                segments, n - 1
            );
        }

    void modifyTickets(id_t startStation, id_t endStation, const TicketDelta& deltas) {
        auto interval = getInterval(startStation, endStation);
        segmentsIntervalManip->modify(interval.first, interval.second, deltas);
    }

    Segment queryTickets(id_t startStation, id_t endStation) {
        auto interval = getInterval(startStation, endStation);
        return segmentsIntervalManip->query(interval.first, interval.second);
    }

    void display() {
        segmentsIntervalManip->apply();
        std::cout << "\n----\n" << std::endl;
        std::cout << "id: " << id << std::endl;
        std::cout << "nStation: " << n << std::endl;
        for (size_t i = 0; i < n; ++ i) {
            std::cout << "\nStation #" << i << ": " << std::endl;
            std::cout << "\tStation id: " << stations[i] << std::endl;
            if (i) std::cout << "\tDistance: " << distance[i - 1] << " km" << std::endl;
            if (i) std::cout << "\tArrive time: " << std::string(arriveTime[i - 1]) << std::endl;
            if (i < n - 1) std::cout << "\tLeave time: " << std::string(leaveTime[i]) << std::endl;
            if (i < n - 1) {
                std::cout << "\tTicket information: \n" << std::endl;
                segments[i].display();
                // std::cout << "#####################" << std::endl;
            }
        }
        std::cout << "\n---\n" << std::endl;
    }
};

}   // TrainBoom

#endif
