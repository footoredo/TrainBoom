#include "TrainBoom.hpp"
using namespace TrainBoom;

int main() {
    TrainBoom::TrainBoom trainBoom;
    trainBoom.loadFromCSV("test/test.csv");
    trainBoom.insertUser(User());
    // std::cout << trainBoom.toString() << std::endl;

    const util::vector<std::string> usersList = trainBoom.listUsers();
    for (const auto& userId: usersList)
        std::cout << userId << std::endl;

    std::string diaozhou = trainBoom.idByStationName("吊州");
    std::string shanghaihongqiao = trainBoom.idByStationName("上海虹桥");
    const auto& vec = trainBoom.station(diaozhou).query(shanghaihongqiao, Datetime::parse("2017/3/28"));
    for (const auto routeInterval: vec) {
        auto& route = trainBoom.route(routeInterval.routeId);
        std::cout << route.queryTickets(routeInterval.l, routeInterval.r).toJson().toString() << std::endl;
    }
}
