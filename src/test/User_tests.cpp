#include "User.hpp"
#include "util.hpp"

using namespace TrainBoom;

int main() {
    Json info("user");
//    info["username"] = std::string("footoredo");
    info["username"] = "footoredo";
    info["gender"] = 1;
    info["isRoot"] = true;
//    std::cout << info["username"].as<std::string>() << std::endl;
    User user(info);
//    return 0;
    user.addOrder(Id("Order"));

    std::cout << user.toJson().toString() << std::endl;
    std::cout << user.getOrderJson().toString() << std::endl;

    info["isRoot"] = false;
    user.update(info);
    std::cout << user.toJson().toString() << std::endl;
}
