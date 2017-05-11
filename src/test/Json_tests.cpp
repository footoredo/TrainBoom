#include "util/Json.hpp"
#include "util/BinaryFile.hpp"
#include <iostream>
using namespace trainBoom;

int main() {
    util::Json json("test");
    json.Parse("{\"username\":\"footoredo\",\"1\":[1,2,3]}");
    std::cout << json.getType() << std::endl;

    json["eat"] = "fish";
    json["see"] = 5.5;

    json["hhh"].SetObject();
    json["hhh"][std::string("ggg")] = false;
    json["hhh"][std::string("ttt")] = 0.15;

    std::cout << json.toString() << std::endl;

    auto bfp = util::make_stupid<BinaryFile>("json.dat");
    try {
        json.write(bfp);
        bfp->Close();
    }
    catch (const exception& e) {
        std::cout << e.what();
    }

    util::Json newjson("test");
    try {
        newjson.read(bfp);
    }
    catch (const exception& e) {
        std::cout << e.what();
    }
    std::cout << newjson.toString() << std::endl;

    util::Json noTypeJson;
    noTypeJson["123"] = "456";
    std::cout << noTypeJson.toString() << std::endl;
}
