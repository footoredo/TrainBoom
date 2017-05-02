#include "User.hpp"
using namespace TrainBoom;

int main() {
    User user;
    user.addOrder(Id("Order"));

    std::cout << user.toJson().toString() << std::endl;
    std::cout << user.getOrderJson().toString() << std::endl;
}
