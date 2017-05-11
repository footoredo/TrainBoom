#ifndef TRAINBOOM_ORDER_HPP
#define TRAINBOOM_ORDER_HPP

#include <string>
#include "util/Json.hpp"
#include "Id.hpp"

namespace TrainBoom {
    struct Order {
        std::string routeId;
        std::string startStationId, endStationId;
        std::string ticketType;
        double ticketPrice;  // Total price.
        unsigned ticketNumber;

        Id id;

        Order(std::string routeId,
                std::string startStationId, std::string endStationId,
                std::string ticketType,
                double ticketPrice, unsigned ticketNumber):
            routeId(routeId), startStationId(startStationId),
            endStationId(endStationId), ticketType(ticketType),
            ticketPrice(ticketPrice), ticketNumber(ticketNumber),
            id("Order") {}

        std::string getId() const {
            return id;
        }

        Json toJson() const {
            Json json("order", id);
            json["routeId"] = routeId;
            json["startStationId"] = startStationId;
            json["endStationId"] = endStationId;
            json["ticketType"] = ticketType;
            json["ticketPrice"] = ticketPrice;
            json["ticketNumber"] = ticketNumber;
            return json;
        }
    };
}

#endif
