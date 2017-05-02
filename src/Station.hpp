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
			util::map<Id, util::set<Id>> toStation;
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
			Station(const std::string& name): name(name), toStation(), id("Station") {}

			Id getId() const noexcept {
				return id;
			}

			std::string getName() const noexcept {
				return name;
			}

			void add(Id stationId, Id routeId){
				if (!toStation[stationId].insert(routeId).second) throw add_routeId_failed(); // assuming return value is pair<iterator,bool>
			}

			void del(Id stationId, Id routeId){
				try {
					toStation[stationId].erase(routeId);
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

			const util::set<Id>& query(Id stationId){
				return toStation[stationId];
			}

	};




}

#endif
