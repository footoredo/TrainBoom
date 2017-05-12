#include "Order.hpp"
#include "DataManager2.hpp"
#include <string>
using namespace trainBoom;

int main() {
    DataManager::init();
    Order order(RouteInterval("routeId", "route", 0, 1), "ssId", "esId", "airplane", 99.8, 5);
    // std::cout << order.toJson().toString() << std::endl;
    // std::cout << order.toJson().toString() << std::endl;

    order.save();
    std::cout << "save done." << std::endl;
    std::string key = DataManager::finish();
    DataManager::init();
    DataManager::load(key);

    Order newOrder(Order::load(order.getId()));
    // std::cout << newOrder.toJson().toString() << std::endl;
    assert(order.toJson().toString() == newOrder.toJson().toString());
    std::cout << "save & load test passed!" << std::endl;

    std::cout << "all tests passed!" << std::endl;
}
