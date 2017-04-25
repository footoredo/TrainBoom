#ifndef TRAINBOOM_HPP
#define TRAINBOOM_HPP

#include <iostream>
#include "util.hpp"
#include "exception.hpp"
#include "Ticket.hpp"
#include "Segment.hpp"
#include "Route.hpp"
#include "User.hpp"

namespace TrainBoom {

class TrainBoom {
private:
	util::map <Id, User> users;
	util::map <Id, Train> trains;
	util::map <Id, Station> stations;
public:
	class id_not_exist : public exception {
    public:
    	id_not_exist() : exception(
    		"id_not_exist",
    		"Your id does not exist!!!") {}
    };
	User& getUser(Id id)
	{
		if(users.count(id) == 0) throw id_not_exist;
		return users[id];
	}
	Train& getTrain(Id id)
	{
		if(trains.count(id) == 0) throw id_not_exist;
		return trains[id];
	}
	Station& getStation(Id id)
	{
		if(stations.count(id) == 0) throw id_not_exist;
		return stations[id];
	}
};

}   // TrainBoom

#endif
