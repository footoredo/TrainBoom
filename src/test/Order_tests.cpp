#include "Order.hpp"
using namespace TrainBoom;

int main() {
    Order order("routeId", "ssId", "esId", "airplane", 99.8, 5);
    std::cout << order.toJson().toString() << std::endl;
}
