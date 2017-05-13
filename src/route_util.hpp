#ifndef TRAINBOOM_ROUTE_UTIL_HPP
#define TRAINBOOM_ROUTE_UTIL_HPP

#include "util/Json.hpp"
#include "util/stupid_ptr.hpp"
#include "util/Datetime.hpp"
#include "DataManager2.hpp"
#include "Id.hpp"
#include <string>

namespace trainBoom {
    struct RouteInterval {
	    std::string routeId, routeName;
	    unsigned l, r;
		std::string id;
		RouteInterval(std::string routeId, std::string routeName, unsigned l, unsigned r):
			routeId(routeId), routeName(routeName), l(l), r(r), id(Id("RouteInterval")) {}
		RouteInterval(const Json& json) {
			if (json.getId() != "") {
				id = json.getId();
			}
			else {
				id = Id("RouteInterval");
			}
			routeId = json["routeId"].as<std::string>();
			routeName = json["routeName"].as<std::string>();
			l = json["l"].as<unsigned>();
			r = json["r"].as<unsigned>();
		}
		// RouteInterval(std::string id, stupid_ptr<BinaryFile> bfp): RouteInterval(Json().read(id, bfp)) {}

        static RouteInterval load(std::string id) {
            return RouteInterval(DataManager::getJson(id));
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
			json["routeName"] = routeName;
	        json["l"] = l;
	        json["r"] = r;
			return json;
	    }
	    Json toJsonEmpty() const {
	        Json json;
	        json["routeId"] = routeId;
			json["routeName"] = routeName;
	        json["l"] = l;
	        json["r"] = r;
			return json;
	    }
		void save() const {
			// std::cout << "RouteInterval {" << std::endl;
			// toJson().write(DataManager::getFile(id));
			// std::cout << "RouteInterval }" << std::endl;
            DataManager::save(toJson());
		}
	};

    struct RouteKey {
        std::string stationName;
        Datetime datetime;

        RouteKey(std::string stationName, Datetime datetime):
            stationName(stationName), datetime(datetime) {}
        RouteKey(std::string routeKey) {
            std::stringstream ss(routeKey);
			std::string tmpDatetime;
            ss >> stationName >> tmpDatetime;
			datetime = Datetime::parse(tmpDatetime);
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
}

#endif
