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
	util::map <Id, User> users;
	util::map <Id, Route> routes;
	util::map <Id, Station> stations;
	Id id;

public:
	class id_not_exist : public exception {
    public:
    	id_not_exist() : exception(
    		"id_not_exist",
    		"Your id does not exist!!!") {}
    };
	TrainBoom(): id("TrainBoom") {}

	User& user(Id id)
	{
		if(users.count(id) == 0) throw id_not_exist();
		return users.at(id);
	}
	Route& route(Id id)
	{
		if(routes.count(id) == 0) throw id_not_exist();
		return routes.at(id);
	}
	Station& station(Id id)
	{
		if(stations.count(id) == 0) throw id_not_exist();
		return stations.at(id);
	}
	void addUser(const User& user) {
		users.insert(util::make_pair(user.getId(), user));
		// users[user.getId()] = user;
	}
	void addTrain(const Train& train) {
		trains.insert(util::make_pair(train.getId(), train));
		// trains[train.getId()] = train;
	}
	void addStation(const Station& station) {
		stations.insert(util::make_pair(station.getId(), station));
		// stations[station.getId()] = station;
	}

	std::string toString() {
		std::stringstream ss;
		ss << "users " << users.size() << '\n';
		for (const auto& user: users) {
			ss << "user " << user.first << '\n';
		}
		ss << "trains " << trains.size() << '\n';
		for (const auto& train: trains) {
			ss << "train " << train.first << '\n';
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
