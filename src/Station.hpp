#ifndef TRAINBOOM_STATION_HPP
#define TRAINBOOM_STATION_HPP
#include <cstring>
#include <string>
// #include <set>
// #include "util.hpp"
#include "util/map.hpp"
#include "util/vector.hpp"
#include "util/Datetime.hpp"
// #include "util/set.hpp"
#include "DataManager2.hpp"
#include "Id.hpp"
#include "util/Json.hpp"
#include "route_util.hpp"
#include <sstream>

namespace trainBoom {
	// struct RouteInterval;
	class Station {
		private:
			std::string name;
            util::map<std::string, util::map<std::string, RouteInterval>> routesMap;
			std::string id;

		public:
            unsigned routeCnt;
			class add_routeId_failed : public exception{
			public:
				add_routeId_failed():exception("add routeId failed","wtf") {};
			};
			class add_trainId_failed : public exception{
			public:
				add_trainId_failed():exception("add trainId failed","wtf") {};
			};
			class delete_routeId_failed : public exception{
			public:
				delete_routeId_failed():exception("delete routeId failed","wtf") {};
			};
			class delete_trainId_failed : public exception{
			public:
				delete_trainId_failed():exception("delete trainId failed","wtf") {};
			};
			Station(const std::string& name): name(name), id(Id("Station")) {}
            Station(const Json& json) {
				if (json.getId() != "") {
					id = json.getId();
				}
				else {
					id = Id("Station");
				}
                name = json["name"].as<std::string>();
				if (json.HasMember("routesMap")) {
					routesMap = util::map<std::string, util::map<std::string, RouteInterval>>::load(json["routesMap"]);
				}
            }

			static Station load(std::string id) {
				return Station(DataManager::getJson(id));
			}

			/*
			Station(std::string id, stupid_ptr<BinaryFile> bfp): id(id) {
				Json tmp; tmp.read(bfp);
				name = tmp["name"].as<std::string>();
				std::string routesMapId = tmp["routesMap"].as<std::string>();
				routesMap.read(routesMapId, DataManager::getFile(routesMapId));
			}
			*/

            void update(const Json& json) {
                if (json.HasMember("name"))
                    name = json["name"].as<std::string>();
            }

            std::string getId() const noexcept {
				return id;
			}

			std::string getName() const noexcept {
				return name;
			}

			void add(const std::string& stationName, const RouteInterval& routeInterval){
                ++ routeCnt;
				routesMap[stationName].insert(util::make_pair(routeInterval.routeName, routeInterval)).second; // assuming return value is pair<iterator,bool>
			}

			void del(const std::string& stationName, const RouteInterval& routeInterval) {
				try {
					auto& routes = routesMap[stationName];
					// std::cout << "done" << std::endl;
					routes.erase(routes.find(routeInterval.routeName));
				}
				catch (const invalid_iterator& e) {
					throw delete_routeId_failed();
				}
			}

			/*void add(Id stationId, Id trainId){
				if (!map[stationId].insert(trainId).second) throw add_trainId_failed();
			}

			void del(Id stationId, Id trainId){
				if (map[stationId].erase(trainId)<1) throw delete_trainId_failed();
			}*/

			util::vector<RouteInterval> query(const std::string& stationName) const {
                util::vector<RouteInterval> ret;
				for (const auto& item: routesMap.at(stationName)) {
                    ret.push_back(item.second);
                }
                return ret;
			}

			RouteInterval query(const std::string& stationName, std::string routeName) const {
                return routesMap.at(stationName).at(routeName);
			}

            Json toJson() const {
                Json json("station", id);
                json["name"] = name;
				json["routesMap"] = routesMap.getId();
                return json;
            }

			void save() const {
				// toJson().write(DataManager::getFile(id));
				DataManager::save(toJson());
				routesMap.save();
			}
/*
            Json queryJson(const std::string& stationId, const Datetime& date) const {
                Json ret("routesList");
                ret["routes"].SetArray();
                for (const auto& id: query(stationId, date)) {
                    ret["routes"].PushBack(id);
                }
                return ret;
            }*/

	};

}

#endif
