#ifndef TRAINBOOM_TRAIN_HPP
#define TRAINBOOM_TRAIN_HPP
#include <cstring>
#include <string>
#include "util.hpp"

namespace TrainBoom {
	class Station {
		private:
			id_t id;
			std::string name;
			util::map<id_t, util::set<id_t>> toStation;

		public:
			class add_routeId_failed : public exception{
				add_routeId_failed():exception("add routeId failed","wtf");
			}
			class add_trainId_failed : public exception{
				add_trainId_failed():exception("add trainId failed","wtf");
			}
			class delete_routeId_failed : public exception{
				delete_routeId_failed():exception("delete routeId failed","wtf");
			}
			class delete_trainId_failed : public exception{
				delete_trainId_failed():exception("delete trainId failed","wtf");
			}
			Station(id_t id, const std::string& name):id(id), name(name), toStation() {}
			
			
			id_t getId() const noexcept {
				return id;
			}
			
			std::string getName() const noexcept {
				return name;
			}
			
			void add(id_t stationId, id_t routeId){
				if (!map[stationId].insert(routeId).second) throw add_routeId_failed(); // assuming return value is pair<iterator,bool>
			}
			
			void del(id_t stationId, id_t routeId){
				if (map[stationId].erase(routeId)<1) throw delete_routeId_failed(); // assuming return value is size_t(number of elements erased)
			}
			
			void add(id_t stationId, id_t trainId){
				if (!map[stationId].insert(trainId).second) throw add_trainId_failed(); 
			}
			
			void del(id_t stationId, id_t trainId){
				if (map[stationId].erase(trainId)<1) throw delete_trainId_failed();
			}
			
			const util::set<id_t>& query(id_t stationId){
				return map[stationId];
			}

	}




}

#endif
