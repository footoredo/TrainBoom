#ifndef TRAINBOOM_TRAIN_HPP
#define TRAINBOOM_TRAIN_HPP

#include "util.hpp"
#include "Route.hpp"
#include <string>

namespace TrainBoom {

class Train {
    friend class Route;

private:
    Id id;
    std::string name;

public:
    Route route;

    Train(Id id, const std::string& name, const Route& route):
        id(id), name(name), route(route) {}

    Id getId() const noexcept {
        return id;
    }

    std::string getName() const noexcept {
        return name;
    }

    void setName(const std::string& _name) noexcept {
        name = _name;
    }

    util::Datetime::Datetime getRunningDay() const noexcept {
        return route.getRunningDay();
    }
};

} // TrainBoom

#endif
