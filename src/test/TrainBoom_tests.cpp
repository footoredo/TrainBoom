#include "TrainBoom.hpp"
#include "DataManager2.hpp"
using namespace trainBoom;

int main() {
    TrainBoom trainBoom;
    DataManager::init();
    trainBoom.loadFromCSV("test/test.csv");
    trainBoom.insertUser(User());
    Json info("user");
//    info["username"] = std::string("footoredo");
    info["username"] = "footoredo";
    info["salt"] = "iamsalt";
    info["password"] = "password";
    info["gender"] = 1;
    info["isRoot"] = true;
//    std::cout << info["username"].as<std::string>() << std::endl;
    User user(info);
    trainBoom.insertUser(user);
    // std::cout << trainBoom.toString() << std::endl;

    const util::vector<std::string> usersList = trainBoom.listUsers();
    for (const auto& userId: usersList)
        std::cout << userId << std::endl;

    std::string diaozhou = trainBoom.idByStationName("吊州");
    std::string shanghaihongqiao = trainBoom.idByStationName("上海虹桥");
    std::cout << "find id done." << std::endl;
    const auto& vec = trainBoom.station(diaozhou).query("上海虹桥");
    for (const auto routeInterval: vec) {
        auto& route = trainBoom.route(routeInterval.routeId);
        // std::cout << route.queryTickets(Route::startDate, routeInterval.l, routeInterval.r).toJson().toString() << std::endl;
    }

    std::cout << "start saving." << std::endl;
    trainBoom.save();
    std::cout << "save done." << std::endl;
    // return 0;

    std::string key = DataManager::finish();
    DataManager::init();
    DataManager::load(key);

    TrainBoom newTrainBoom(TrainBoom::load(trainBoom.getId()));
    assert(newTrainBoom.getId() == trainBoom.getId());
    assert(newTrainBoom.listUsers() == usersList);
    for (const auto& userId: newTrainBoom.listUsers()) {
        assert(newTrainBoom.user(userId).toJson().toString() ==
                trainBoom.user(userId).toJson().toString());
    }
    assert(newTrainBoom.listStations() == trainBoom.listStations());
    for (const auto& stationId: newTrainBoom.listStations()) {
        // std::cout << "done check" << std::endl;
        if (newTrainBoom.station(stationId).toJson().toString() != trainBoom.station(stationId).toJson().toString()) {
            std::cout << newTrainBoom.station(stationId).toJson().toString() << std::endl;
            std::cout << trainBoom.station(stationId).toJson().toString() << std::endl;
        }
        assert(newTrainBoom.station(stationId).toJson().toString() ==
                trainBoom.station(stationId).toJson().toString());
    }
    assert(newTrainBoom.listRoutes() == trainBoom.listRoutes());
    for (const auto& routeId: newTrainBoom.listRoutes()) {
        assert(newTrainBoom.route(routeId).toJson().toString() ==
                trainBoom.route(routeId).toJson().toString());
    }
    assert(newTrainBoom.idByStationName("吊州") == diaozhou);
    assert(newTrainBoom.idByStationName("上海虹桥") == shanghaihongqiao);
    const auto& vec2 = newTrainBoom.station(diaozhou).query("上海虹桥");
    assert(vec.size() == vec2.size());
    for (unsigned i = 0; i < vec.size(); ++ i) {
        assert(vec[i].l == vec2[i].l && vec[i].r == vec2[i].r && vec[i].routeId == vec2[i].routeId);
        auto& route = trainBoom.route(vec[i].routeId);
        auto& route2 = newTrainBoom.route(vec2[i].routeId);
        assert(route.queryTickets(Route::startDate, vec[i].l, vec[i].r).toJson().toString()
            == route2.queryTickets(Route::startDate, vec[i].l, vec[i].r).toJson().toString());
    }
    // assert(newTrainBoom.station(diaozhou).query(shanghaihongqiao, Datetime::parse("2017/3/28")) == vec);

    std::cout << "save & load test done!" << std::endl;
}
