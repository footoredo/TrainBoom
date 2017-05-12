#include "API.hpp"
#include "TrainBoom.hpp"
#include "DataManager.hpp"

using namespace trainBoom;

int main(int argc, char **argv) {
    DataManager::init();
    auto trainBoom = make_stupid<trainBoom::TrainBoom>();
    if (argc > 1) trainBoom->load(std::string(argv[1]));
    else trainBoom->loadFromCSV("test/test.csv");
//    trainBoom->listRouteCnt();
    API::APIServer apiServer(trainBoom, 3000);
//    API::APIServer apiServer(util::make_stupid<TrainBoom::TrainBoom>(), 3000);
    apiServer.run(1);

//    std::cout << trainBoom->getId() << std::endl;
//    trainBoom->save();
//    apiServer.shutdown();
}
