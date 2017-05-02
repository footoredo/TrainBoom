#include "TrainBoom.hpp"
using namespace TrainBoom;

int main() {
    TrainBoom::TrainBoom trainBoom;
    trainBoom.addUser(User());
    std::cout << trainBoom.toString() << std::endl;
}
