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
#include <sstream>

namespace trainBoom {
	// struct RouteInterval;
	struct RouteInterval {
	    std::string routeId;
	    unsigned l, r;
		std::string id;
		RouteInterval(std::string routeId, unsigned l, unsigned r):
			routeId(routeId), l(l), r(r), id(Id("RouteInterval")) {}
		RouteInterval(std::string id, stupid_ptr<BinaryFile> bfp): id(id) {
			Json tmp; tmp.read(bfp);
			routeId = tmp["routeId"].as<std::string>();
			l = tmp["l"].as<unsigned>();
			r = tmp["r"].as<unsigned>();
		}
	    bool operator<(const RouteInterval& other) const {
	        if (routeId == other.routeId)
	            if (l == other.l)
	                return r < other.r;
	            else
	                return l < other.l;
	        else
	            return routeId < other.routeId;
	    }
		std::string getId() const {
			return id;
		}
	    Json toJson() const {
	        Json json("routeInterval", id);
	        json["routeId"] = routeId;
	        json["l"] = l;
	        json["r"] = r;
			return json;
	    }
		void save() const {
			// std::cout << "RouteInterval {" << std::endl;
			toJson().write(DataManager::getFile(id));
			// std::cout << "RouteInterval }" << std::endl;
		}
	};

    struct RouteKey {
        std::string stationName;
        Datetime datetime;
    
        RouteKey(std::string stationName, Datetime datetime):
            stationId(stationName), datetime(datetime) {}
        RouteKey(std::string routeKey) {
            std::stringstream ss(routeKey);
            ss >> stationId >> datetime;
        }

        operator std::string() const {
            std::stringstream ss;
            ss << stationName << " " << datetime;
            return ss.str();
        }

        bool operator<(const RouteKey& other) const {
            if (stationName == other.stationName)
                return datetime < other.datetime;
            else
                return stationName < other.stationName;
        }
    };

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
				if (!routesMap[RouteKey(stationName)].insert(routeInterval).second) throw add_routeId_failed(); // assuming return value is pair<iterator,bool>
			}

			void del(const std::string& stationId, const util::Datetime::Datetime& date, const RouteInterval& routeInterval) {
				try {
					routesMap[stationId][date].erase(routeInterval);
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

			util::vector<RouteInterval> query(const std::string& stationId, const util::Datetime::Datetime& date) const {
                util::vector<RouteInterval> ret;
                if (!routesMap.count(stationId) ||
                        !routesMap.at(stationId).count(date))
                    return ret;
				for (const auto& routeInterval: routesMap.at(stationId).at(date)) {
                    ret.push_back(routeInterval);
                }
                return ret;
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
