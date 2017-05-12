#ifndef TRAINBOOM_ORDER_HPP
#define TRAINBOOM_ORDER_HPP

#include <string>
#include "util/Json.hpp"
#include "util/Datetime.hpp"
#include "Id.hpp"
#include "DataManager.hpp"
#include "route_util.hpp"

namespace trainBoom {
    struct Order {
        RouteInterval routeInterval;
        std::string startStationName, endStationName;
        std::string ticketType;
        double ticketPrice;  // Total price.
        unsigned ticketNumber;

        std::string id;

        Order(RouteInterval routeInterval,
                std::string startStationName, std::string endStationName,
                std::string ticketType,
                double ticketPrice, unsigned ticketNumber):
            routeInterval(routeInterval), startStationName(startStationName),
            endStationName(endStationName), ticketType(ticketType),
            ticketPrice(ticketPrice), ticketNumber(ticketNumber),
            id(Id("Order")) {}

        Order(const Json& json): routeInterval(json["routeInterval"]) {
            if (json.getId() != "") {
                id = json.getId();
            }
            else {
                id = Id("Order");
            }
            startStationName = json["startStationName"].as<std::string>();
            endStationName = json["endStationName"].as<std::string>();
            ticketType = json["ticketType"].as<std::string>();
            ticketPrice = json["ticketPrice"].as<double>();
            ticketNumber = json["ticketNumber"].as<unsigned>();
        }

        Order(std::string id, stupid_ptr<BinaryFile> bfp): Order(Json().read(id, bfp)) {}

        std::string getId() const {
            return id;
        }

        Json toJson() const {
            Json json("order", id);
            json["routeInterval"] = routeInterval.toJson();
            json["startStationName"] = startStationName;
            json["endStationName"] = endStationName;
            json["ticketType"] = ticketType;
            json["ticketPrice"] = ticketPrice;
            json["ticketNumber"] = ticketNumber;
            return json;
        }

        void save() const {
            toJson().write(DataManager::getFile(id));
        }
    };
}

#endif
