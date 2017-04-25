#include "util/Json.hpp"
#include <iostream>
using namespace TrainBoom;

int main() {
    util::Json json("test");
    std::cout << json.getName() << std::endl;

    json["eat"] = "fish";
    json["see"] = 5.5;

    std::cout << json.toString() << std::endl;
}
