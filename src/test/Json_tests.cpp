#include "util/Json.hpp"
#include "BinaryFile.hpp"
#include <iostream>
using namespace TrainBoom;

int main() {
    util::Json json("test");/*
    json.Parse("{\"username\":\"footoredo\",\"1\":[1,2,3]}");
    std::cout << json.getType() << std::endl;

    json["eat"] = "fish";
    json["see"] = 5.5;

    json["hhh"].SetObject();
    json["hhh"][std::string("ggg")] = false;
    json["hhh"][std::string("ttt")] = 0.15;

    std::cout << json.toString() << std::endl;
*/
    BinaryFile bf("json.dat");
    try {
        json.write(bf);
    }
    catch (const exception& e) {
        std::cout << e.what();
    }

    util::Json newjson("test");
    try {
        newjson.read(bf);
    }
    catch (const exception& e) {
        std::cout << e.what();
    }
    std::cout << newjson.toString() << std::endl;
}
