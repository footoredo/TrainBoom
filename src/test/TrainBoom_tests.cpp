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
}
