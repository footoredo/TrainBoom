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
			Station(id_t id, const std::string& name):id(id), name(name), toStation() {}
			
			
			id_t getId() const noexcept {
				return id;
			}
			
			std::string getName() const noexcept {
				return name;
			}
			
			void add(id_t stationId, id_t routeId){
				map[stationId].insert(routeId);
			}
			
			void del(id_t stationId, id_t routeId){
				map[stationId].erase(routeId);
			}
			
			void add(id_t stationId, id_t trainId){
				map[stationId].insert(trainId);
			}
			
			void del(id_t stationId, id_t trainId){
				map[stationId].erase(trainId);
			}
			
			const util::set<id_t>& query(id_t stationId){
				return map[stationId];
			}

	}




}

#endif
