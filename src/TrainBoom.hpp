#ifndef TRAINBOOM_HPP
#define TRAINBOOM_HPP

#include <iostream>
#include "exception.hpp"
#include "Ticket.hpp"
#include "Segment.hpp"
#include "Route.hpp"
#include "User.hpp"
#include "Train.hpp"
#include "Station.hpp"
#include "util.hpp"

namespace TrainBoom {

class TrainBoom {
private:
	util::map <std::string, User> users;
	util::map <std::string, Route> routes;
	util::map <std::string, Station> stations;
	Id id;

public:
	class id_not_exist : public exception {
    public:
    	id_not_exist(std::string type) : exception(
    		"id_not_exist",
    		"Your " + type + " id does not exist!!!") {}
    };
	TrainBoom(): id("TrainBoom") {}

    std::string getId() const {
        return id;
    }

	User& user(std::string id)
	{
//        std::cout << id << " " << users.count(id) << std::endl;
		if(users.count(id) == 0) throw id_not_exist("user");
		return users.at(id);
	}
	Route& route(std::string id)
	{
		if(routes.count(id) == 0) throw id_not_exist("route");
		return routes.at(id);
	}
	Station& station(std::string id)
	{
		if(stations.count(id) == 0) throw id_not_exist("station");
		return stations.at(id);
	}
	void insertUser(const User& user) {
		users.insert(util::make_pair(user.getId(), user));
		// users[user.getId()] = user;
	}
	void insertRoute(const Route& route) {
		routes.insert(util::make_pair(route.getId(), route));
		// trains[train.getId()] = train;
	}
	void insertStation(const Station& station) {
		stations.insert(util::make_pair(station.getId(), station));
		// stations[station.getId()] = station;
	}

    util::vector<std::string> listUsers() {
        util::vector<std::string> usersList;
        for (const auto& item: users)
            usersList.push_back(item.first);
        return usersList;
    }

    util::vector<std::string> listStations() {
        util::vector<std::string> stationsList;
        for (const auto& item: stations)
            stationsList.push_back(item.first);
        return stationsList;
    }

    util::vector<std::string> listRoutes() {
        util::vector<std::string> routesList;
        for (const auto& item: routes)
            routesList.push_back(item.first);
        return routesList;
    }

    void startRoute(const Route& route) {
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(route.information(j).getStationId()).
                    add(route.information(i).getStationId(),
                            route.information(j).getDate(),
                            route.getId());
            }
    }

    void stopRoute(const Route& route) {
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(route.information(j).getStationId()).
                    del(route.information(i).getStationId(),
                            route.information(j).getDate(),
                            route.getId());
            }
    }

	std::string toString() {
		std::stringstream ss;
		ss << "users " << users.size() << '\n';
		for (const auto& user: users) {
			ss << "user " << user.first << '\n';
		}
		ss << "routes " << routes.size() << '\n';
		for (const auto& route: routes) {
			ss << "route " << route.first << '\n';
		}
		ss << "stations " << stations.size() << '\n';
		for (const auto& station: stations) {
			ss << "station " << station.first << '\n';
		}
		return ss.str();
	}

};

}   // TrainBoom

#endif
