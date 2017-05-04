#include "util/Json.hpp"
#include <iostream>
using namespace TrainBoom;

int main() {
    util::Json json("test");
    json.Parse("{\"username\":\"footoredo\",\"1\":[1,2,3]}");
    std::cout << json.getType() << std::endl;

    json["eat"] = "fish";
    json["see"] = 5.5;

    std::cout << json.toString() << std::endl;
}
