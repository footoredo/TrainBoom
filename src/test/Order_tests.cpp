#include "Order.hpp"
#include "DataManager.hpp"
using namespace trainBoom;

int main() {
    DataManager::init();
    Order order("routeId", "ssId", "esId", "airplane", 99.8, 5);
    std::cout << order.toJson().toString() << std::endl;

    order.save();
    std::cout << "save done." << std::endl;

    Order newOrder(order.getId(), DataManager::getFile(order.getId()));
    assert(order.toJson().toString() == newOrder.toJson().toString());
    std::cout << "save & load test passed!" << std::endl;
}
