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
#include <sstream>

namespace TrainBoom {

class TrainBoom {
private:
	util::map <std::string, User> users;
	util::map <std::string, Route> routes;
	util::map <std::string, Station> stations;
	Id id;

    util::map <std::string, std::string> usernameMap, stationNameMap;

	static double readPrice(std::string buffer) {
		if (buffer == "-") return 0;
		else {
			std::stringstream ss(buffer);
			char r, m, b;
			double price;
			// std::cout << buffer << std::endl;
			ss >> r >> m >> b >> price;
			// std::cout << r << " " << m << " " << b << "  " << price << std::endl;
			return price;
		}
	}

public:
	class id_not_exist : public exception {
    public:
    	id_not_exist(std::string type) : exception(
    		"id_not_exist",
    		"Your " + type + " id does not exist!!!") {}
    };
	class username_not_found: public exception {
    public:
    	username_not_found(std::string username): exception(
    		"username_not_found",
    		"Your username [" + username + "] is not found!!!") {}
    };
	class username_exists : public exception {
    public:
    	username_exists(std::string username): exception(
    		"username_not_found",
    		"Your username [" + username + "] already exists!!!") {}
    };
	class stationName_not_found: public exception {
    public:
    	stationName_not_found(std::string stationName): exception(
    		"stationName_not_found",
    		"Your stationName [" + stationName + "] is not found!!!") {}
    };
	class stationName_exists : public exception {
    public:
    	stationName_exists(std::string stationName): exception(
    		"stationName_not_found",
    		"Your stationName [" + stationName + "] already exists!!!") {}
    };
    class route_already_running: public exception {
        public:
            route_already_running(): exception(
                    "route_already_running",
                    "The route you tried to start is already running!") {}
    };
    class route_not_running: public exception {
        public:
            route_not_running(): exception(
                    "route_not_running",
                    "The route you tried to stop is not running!") {}
    };
	TrainBoom(): id("TrainBoom") {}

	void loadFromCSV(std::string csvFile) {
		CSV csv; csv.load(csvFile);
		int routeCnt = 0;
		for (int i = 1; i <= csv.size(); ) {
			++ routeCnt;
			std::cout << "Importing Route #" << routeCnt << std::endl;
			assert(csv.data(i, 2) == "");
			Json route("route");
			route["name"] = csv.data(i, 1);
			route["n"] = 1;
			route["informations"].SetArray();
			route["segments"].SetArray();

			++ i;
			int ticketCount = csv.size(i) - 5;
			stupid_array<std::string> ticketName(new std::string[ticketCount], ticketCount);
			for (int j = 0; j < ticketCount; ++ j)
				ticketName[j] = csv.data(i, 6 + j);

			++ i;
			int cnt = 0;
			while (i <= csv.size() && csv.data(i, 2) != "") {
				Json information("information");

				int flags = 0;

				std::string stationName(csv.data(i, 1)), stationId;
				try {
					stationId = idByStationName(stationName);
				}
				catch (const stationName_not_found& e) {
					Json stationJson("station");
					stationJson["name"] = stationName;
					Station station(stationJson);
					insertStation(station);
					stationId = station.getId();
				}
				information["stationId"] = stationId;

				std::string date(csv.data(i, 2));
				if (csv.data(i, 3) == "起点站") {
					flags |= isStart;
					information["leaveTime"] = Datetime::parse(date + " " + csv.data(i, 4));
				}
				else if (csv.data(i, 4) == "终到站") {
					flags |= isEnd;
					information["arriveTime"] = Datetime::parse(date + " " + csv.data(i, 3));
				}
				else {
					information["arriveTime"] = Datetime::parse(date + " " + csv.data(i, 3));
					information["leaveTime"] = Datetime::parse(date + " " + csv.data(i, 4));
				}

				information["distance"] = std::stoi(csv.data(i, 5));
				information["flags"] = flags;

				route["informations"].PushBack(information);

				if (cnt) {
					Json segment("segment");
					segment["tickets"].SetObject();
					for (int j = 0; j < ticketCount; ++ j) {
						Json attribute("attribute");
						attribute["price"] = readPrice(csv.data(i, 6 + j)) - readPrice(csv.data(i - 1, 6 + j));
						if (csv.data(i, 6 + j) == "-") {
							attribute["nonstop"] = true;
						}
						segment["tickets"][ticketName[j]] = attribute;
					}
					route["segments"].PushBack(segment);
				}

				++ i;
				++ cnt;
			}

			route["n"] = cnt;
			// std::cout << route.toString() << std::endl;
			Route tmp(route);
			insertRoute(tmp);
			startRoute(routes[tmp.getId()]);

			// if (i > 150) break;
		}

		std::cout << "Import done." << std::endl;
	}

    std::string getId() const {
        return id;
    }

	User& user(std::string id)
	{
//        std::cout << id << " " << users.count(id) << std::endl;
		if(users.count(id) == 0) throw id_not_exist("user");
		return users.at(id);
	}
    std::string idByUsername(std::string username) const {
        if (!usernameMap.count(username)) {
            throw username_not_found(username);
        }
        return usernameMap.at(username);
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
	std::string idByStationName(std::string stationName) const {
        if (!stationNameMap.count(stationName)) {
            throw stationName_not_found(stationName);
        }
        return stationNameMap.at(stationName);
    }
	void insertUser(const User& user) {
        if (usernameMap.count(user.getUsername()) != 0) {
            throw username_exists(user.getUsername());
        }
		users.insert(util::make_pair(user.getId(), user));
        usernameMap.insert(util::make_pair(user.getUsername(), user.getId()));
		// users[user.getId()] = user;
	}
	void insertRoute(const Route& route) {
		std::cout << "new route [" << route.getName() << "] => " << route.getId() << std::endl;
		routes.insert(util::make_pair(route.getId(), route));
		// trains[train.getId()] = train;
	}
	void insertStation(const Station& station) {
		if (stationNameMap.count(station.getName()) != 0) {
			throw stationName_exists(station.getName());
		}
		std::cout << "new station [" << station.getName() << "] => " << station.getId() << std::endl;
		stations.insert(util::make_pair(station.getId(), station));
		stationNameMap.insert(util::make_pair(station.getName(), station.getId()));
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

    void startRoute(Route& route) {
        if (route.getRunning()) {
            throw route_already_running();
        }
        route.setRunning(true);
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(route.information(j).getStationId()).
                    add(route.information(i).getStationId(),
                            route.information(j).getDate(),
                            RouteInterval(route.getId(), j, i));
            }
    }

    void stopRoute(Route& route) {
        if (!route.getRunning()) {
            throw route_not_running();
        }
        route.setRunning(false);
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(route.information(j).getStationId()).
                    del(route.information(i).getStationId(),
                            route.information(j).getDate(),
                            RouteInterval(route.getId(), j, i));
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
