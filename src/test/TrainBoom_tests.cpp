#include "TrainBoom.hpp"
using namespace TrainBoom;

int main() {
    TrainBoom::TrainBoom trainBoom;
    trainBoom.insertUser(User());
    trainBoom.insertUser(User());
    std::cout << trainBoom.toString() << std::endl;

    const util::vector<Id> usersList = trainBoom.listUsers();
    for (const auto& userId: usersList)
        std::cout << userId << std::endl;
}
