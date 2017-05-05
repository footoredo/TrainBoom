#ifndef TRAINBOOM_STATION_HPP
#define TRAINBOOM_STATION_HPP
#include <cstring>
#include <string>
#include <set>
#include "util.hpp"

namespace TrainBoom {
	class Station {
		private:
			std::string name;
			util::map<std::string, util::map<util::Datetime::Datetime, util::set<std::string>>> routesMap;
			Id id;

		public:
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
			Station(const std::string& name): name(name), id("Station") {}
            Station(const Json& json): id("Station") {
                name = json["name"].as<std::string>();
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

			void add(const std::string& stationId, const util::Datetime::Datetime& date, const std::string& routeId){
				if (!routesMap[stationId][date].insert(routeId).second) throw add_routeId_failed(); // assuming return value is pair<iterator,bool>
			}

			void del(const std::string& stationId, const util::Datetime::Datetime& date, const std::string& routeId) {
				try {
					routesMap[stationId][date].erase(routeId);
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

			util::vector<std::string> query(const std::string& stationId, const util::Datetime::Datetime& date) const {
                util::vector<std::string> ret;
                if (!routesMap.count(stationId) ||
                        !routesMap.at(stationId).count(date))
                    return ret;
				for (const std::string& routeId: routesMap.at(stationId).at(date)) {
                    ret.push_back(routeId);
                }
                return ret;
			}

            Json toJson() const {
                Json json("station", id);
                json["name"] = name;
                return json;
            }

            Json queryJson(const std::string& stationId, const Datetime& date) const {
                Json ret("routesList");
                ret["routes"].SetArray();
                for (const auto& id: query(stationId, date)) {
                    ret["routes"].PushBack(id);
                }
                return ret;
            }

	};

}

#endif
