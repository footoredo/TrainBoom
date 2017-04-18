#ifndef TRAINBOOM_HPP
#define TRAINBOOM_HPP

#include <iostream>
#include "util.hpp"
#include "exception.hpp"
#include "Ticket.hpp"
#include "Segment.hpp" 
#include "Route.hpp" 
#include "User.hpp" 
#include "Train.hpp" 

namespace TrainBoom {

class TrainBoom {
private:
	util::map <id_t, User> users;
	util::map <id_t, Train> trains;
	util::map <id_t, Station> stations;
public:
	class id_not_exist : public exception {
    public:
    	id_not_exist() : exception(
    		"id_not_exist",
    		"Your id does not exist!!!") {}
    };
	User& getUser(id_t id)
	{
		if(users.count(id) == 0) throw id_not_exist;
		return users[id];
	}
	Train& getTrain(id_t id)
	{
		if(trains.count(id) == 0) throw id_not_exist;
		return trains[id];
	}
	Station& getStation(id_t id)
	{
		if(stations.count(id) == 0) throw id_not_exist;
		return stations[id];
	}
};

}   // TrainBoom

#endif
