#ifndef TRAINBOOM_HPP
#define TRAINBOOM_HPP

#include <iostream>
#include "util/map.hpp"
#include "util/stupid_ptr.hpp"
#include "DataManager2.hpp"
#include "User.hpp"
#include "Route.hpp"
#include "Station.hpp"
#include "Blob.hpp"
#include "util/CSV.hpp"
#include <sstream>

namespace trainBoom {

class TrainBoom {
private:
	util::map <std::string, User> users;
	util::map <std::string, Route> routes;
	util::map <std::string, Station> stations;
	std::string id;

    util::map <std::string, Blob> usernameMap, stationNameMap, routeNameMap;

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
	class routeName_not_found: public exception {
    public:
    	routeName_not_found(std::string routeName): exception(
    		"routeName_not_found",
    		"Your routeName [" + routeName + "] is not found!!!") {}
    };
	class routeName_exists : public exception {
    public:
    	routeName_exists(std::string routeName): exception(
    		"routeName_not_found",
    		"Your routeName [" + routeName + "] already exists!!!") {}
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
	TrainBoom(): id(Id("TrainBoom")) {}
	TrainBoom(const Json& tmp) {
		if (tmp.getId() != "") {
			id = tmp.getId();
		}
		else {
			id = Id("TrainBoom");
		}
		// std::cout << tmp.toString() << std::endl;
		std::string usersId = tmp["usersId"].as<std::string>();
		users = util::map<std::string, User>::load(usersId);
		// std::cout << "User loaded" << std::endl;
		std::string routesId = tmp["routesId"].as<std::string>();
		routes = util::map<std::string, Route>::load(routesId);
		// std::cout << "Route loaded" << std::endl;
		std::string stationsId = tmp["stationsId"].as<std::string>();
		stations = util::map<std::string, Station>::load(stationsId);
		// std::cout << "Station loaded" << std::endl;
		std::string usernameMapId = tmp["usernameMapId"].as<std::string>();
		usernameMap = util::map<std::string, Blob>::load(usernameMapId);
		// std::cout << "usernameMap loaded" << std::endl;
		std::string stationNameMapId = tmp["stationNameMapId"].as<std::string>();
		stationNameMap = util::map<std::string, Blob>::load(stationNameMapId);
		// std::cout << "stationNameMap loaded" << std::endl;
		std::string routeNameMapId = tmp["routeNameMapId"].as<std::string>();
		routeNameMap = util::map<std::string, Blob>::load(routeNameMapId);
		// std::cout << "routeNameMap loaded" << std::endl;
	}

    void inside_load(std::string _id) {
        id = _id;
        Json tmp(DataManager::getJson(id));
		std::string usersId = tmp["usersId"].as<std::string>();
		users = util::map<std::string, User>::load(usersId);
		std::string routesId = tmp["routesId"].as<std::string>();
		routes = util::map<std::string, Route>::load(routesId);
		std::string stationsId = tmp["stationsId"].as<std::string>();
		stations = util::map<std::string, Station>::load(stationsId);
		std::string usernameMapId = tmp["usernameMapId"].as<std::string>();
		usernameMap = util::map<std::string, Blob>::load(usernameMapId);
		std::string stationNameMapId = tmp["stationNameMapId"].as<std::string>();
		stationNameMap = util::map<std::string, Blob>::load(stationNameMapId);
		std::string routeNameMapId = tmp["routeNameMapId"].as<std::string>();
		routeNameMap = util::map<std::string, Blob>::load(routeNameMapId);
    }

	static TrainBoom load(std::string id) {
		return TrainBoom(DataManager::getJson(id));
	}

	void loadFromCSV(std::string csvFile) {
		CSV csv; csv.load(csvFile);
		int routeCnt = 0;
		for (int i = 1; i <= csv.size(); ) {
			++ routeCnt;
			// std::cout << "Importing Route #" << routeCnt << std::endl;
			assert(csv.data(i, 2) == "");
			Json route("route");
			route["name"] = csv.data(i, 1);
			route["n"] = 1;
			route["informations"].SetArray();
			route["segments"].SetArray();

			++ i;
			int ticketCount = csv.size(i) - 5;
			while (csv.data(i, ticketCount + 5) == "") -- ticketCount;
			stupid_array<std::string> ticketName(new std::string[ticketCount], ticketCount);
			for (int j = 0; j < ticketCount; ++ j)
				ticketName[j] = csv.data(i, 6 + j);

			++ i;
			int cnt = 0, start = i;
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
				information["stationName"] = stationName;

				std::string date(csv.data(i, 2));
				if (csv.data(i, 3) == "起点站") {
					flags |= isStart;
					information["leaveTime"] = Datetime::parse(date + " " + csv.data(i, 4)) - Datetime::parse(date + " " + csv.data(start, 4));
				}
				else if (csv.data(i, 4) == "终到站") {
					flags |= isEnd;
					information["arriveTime"] = Datetime::parse(date + " " + csv.data(i, 3)) - Datetime::parse(date + " " + csv.data(start, 3));
				}
				else {
					information["arriveTime"] = Datetime::parse(date + " " + csv.data(i, 3)) - Datetime::parse(date + " " + csv.data(start, 3));
					information["leaveTime"] = Datetime::parse(date + " " + csv.data(i, 4)) - Datetime::parse(date + " " + csv.data(start, 4));
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
			try {
				insertRoute(tmp);
				// startRoute(routes[tmp.getId()]);
			}
			catch (const routeName_exists& e) {
				std::cout << "Duplicated route found [" + tmp.getName() << "]" << std::endl;
			}

//			if (i > 150) break;
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
	std::string idByRouteName(std::string routeName) const {
        if (!routeNameMap.count(routeName)) {
            throw routeName_not_found(routeName);
        }
        return routeNameMap.at(routeName);
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
		if (routeNameMap.count(route.getName()) != 0) {
			throw routeName_exists(route.getName());
		}
		// std::cout << "new route [" << route.getName() << "] => " << route.getId() << std::endl;
		routes.insert(util::make_pair(route.getId(), route));
        routeNameMap.insert(util::make_pair(route.getName(), route.getId()));
        startRoute(route.getId());
		// trains[train.getId()] = train;
	}
	void insertStation(const Station& station) {
		if (stationNameMap.count(station.getName()) != 0) {
			throw stationName_exists(station.getName());
		}
		// std::cout << "new station [" << station.getName() << "] => " << station.getId() << std::endl;
		stations.insert(util::make_pair(station.getId(), station));
		stationNameMap.insert(util::make_pair(station.getName(), station.getId()));
		// stations[station.getId()] = station;
	}

	void deleteUser(std::string id) {
		if (!users.count(id))
			throw id_not_exist(id);
		usernameMap.erase(usernameMap.find(users.at(id).getUsername()));
		users.erase(users.find(id));
	}

	void deleteRoute(std::string id) {
		if (!routes.count(id))
			throw id_not_exist(id);
        if (routes.at(id).getRunning()) {
            stopRoute(id);
        }
		routeNameMap.erase(routeNameMap.find(routes.at(id).getName()));
		routes.erase(routes.find(id));
	}

	void deleteStation(std::string id) {
		if (!stations.count(id))
			throw id_not_exist(id);
		stationNameMap.erase(stationNameMap.find(stations.at(id).getName()));
		stations.erase(stations.find(id));
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

    void startRoute(std::string routeId) {
        Route& route = this->route(routeId);
        if (route.getRunning()) {
            throw route_already_running();
        }
        route.setRunning(true);
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(idByStationName(route.information(j).getStationName())).
                    add(route.information(i).getStationName(),
                            RouteInterval(route.getId(), route.getName(), j, i));
            }
    }

    void stopRoute(std::string routeId) {
        Route& route = this->route(routeId);
        if (!route.getRunning()) {
            throw route_not_running();
        }
        route.setRunning(false);
        unsigned n = route.size();
        for (unsigned i = 0; i < n; ++ i)
            for (unsigned j = 0; j < i; ++ j) {
                station(idByStationName(route.information(j).getStationName())).
                    del(route.information(i).getStationName(),
                            RouteInterval(route.getId(), route.getName(), j, i));
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

	void save() {
		Json tmp("TrainBoom", id);
		users.save();
		tmp["usersId"] = users.getId();
		routes.save();
		tmp["routesId"] = routes.getId();
		stations.save();
		tmp["stationsId"] = stations.getId();
		usernameMap.save();
		tmp["usernameMapId"] = usernameMap.getId();
		stationNameMap.save();
		tmp["stationNameMapId"] = stationNameMap.getId();
		routeNameMap.save();
		tmp["routeNameMapId"] = routeNameMap.getId();
		// tmp.write(DataManager::getFile(id));
		DataManager::save(tmp);
	}

    void listRouteCnt() const {
        int cnt = 0;
        for (const auto& item: stations) {
            std::cout << ++cnt << ": " << item.second.routeCnt << std::endl;
        }
    }

};

}   // trainBoom

#endif
