#ifndef TRAINBOOM_STATION_HPP
#define TRAINBOOM_STATION_HPP
#include <cstring>
#include <string>
#include <set>
// #include "util.hpp"
#include "util/map.hpp"
#include "util/vector.hpp"
#include "util/Datetime.hpp"
#include "util/set.hpp"
#include "DataManager.hpp"
#include "Id.hpp"
#include "util/Json.hpp"
#include "route_util.hpp"
#include <sstream>

namespace trainBoom {
	// struct RouteInterval;
	class Station {
		private:
			std::string name;
            util::map<RouteKey, util::map<std::string, RouteInterval>> routesMap;
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
            Station(const Json& json): id(Id("Station")) {
                name = json["name"].as<std::string>();
            }
			Station(std::string id, stupid_ptr<BinaryFile> bfp): id(id) {
				Json tmp; tmp.read(bfp);
				name = tmp["name"].as<std::string>();
				std::string routesMapId = tmp["routesMap"].as<std::string>();
				routesMap.read(routesMapId, DataManager::getFile(routesMapId));
			}

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

			void add(const std::string& stationName, const util::Datetime::Datetime& date, const RouteInterval& routeInterval){
                ++ routeCnt;
				routesMap[RouteKey(stationName, date)].insert(util::make_pair(routeInterval.routeName, routeInterval)).second; // assuming return value is pair<iterator,bool>
			}

			void del(const std::string& stationName, const util::Datetime::Datetime& date, const RouteInterval& routeInterval) {
				try {
					auto& routes = routesMap[RouteKey(stationName, date)];
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

			util::vector<RouteInterval> query(const std::string& stationName, const util::Datetime::Datetime& date) const {
                util::vector<RouteInterval> ret;
				RouteKey routeKey(stationName, date);
				for (const auto& item: routesMap.at(routeKey)) {
                    ret.push_back(item.second);
                }
                return ret;
			}

			RouteInterval query(const std::string& stationName, const util::Datetime::Datetime& date, std::string routeName) const {
                return routesMap.at(RouteKey(stationName, date)).at(routeName);
			}

            Json toJson() const {
                Json json("station", id);
                json["name"] = name;
				json["routesMap"] = routesMap.getId();
                return json;
            }

			void save() const {
				toJson().write(DataManager::getFile(id));
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
