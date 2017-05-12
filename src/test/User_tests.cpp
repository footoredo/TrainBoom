#include "User.hpp"
#include "DataManager2.hpp"

using namespace trainBoom;

int main() {
    DataManager::init();
    Json info("user");
//    info["username"] = std::string("footoredo");
    info["username"] = "footoredo";
    info["salt"] = "iamsalt";
    info["password"] = "password";
    info["gender"] = 1;
    info["isRoot"] = true;
//    std::cout << info["username"].as<std::string>() << std::endl;
    User user(info);
    Order order(RouteInterval("routeId", "route", 0, 2), "ssId", "esId", "airplane", 99.8, 5);
    user.addOrder(order);
//    return 0;
//    user.addOrder(Order("Order"));

    std::cout << user.toJson().toString() << std::endl;
//    std::cout << user.getOrderJson().toString() << std::endl;

    info["isRoot"] = false;
    user.update(info);
    std::cout << user.toJson().toString() << std::endl;

    user.save();
    std::cout << "save done." << std::endl;
    std::string key = DataManager::finish();
    DataManager::init();
    DataManager::load(key);

    User newUser(User::load(user.getId()));
    // std::cout << newUser.toJson().toString() << std::endl;
    assert(newUser.toJson().toString() == user.toJson().toString());
    auto orders = newUser.getOrders();
    assert(orders.size() == 1 && newUser.order(orders[0]).toJson().toString() == order.toJson().toString());
    std::cout << "save & load test passed!" << std::endl;
}
