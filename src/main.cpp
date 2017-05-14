#include "API.hpp"
#include "TrainBoom.hpp"
#include "DataManager.hpp"

using namespace trainBoom;

int main(int argc, char **argv) {
    DataManager::init();
    auto trainBoom = make_stupid<trainBoom::TrainBoom>();
    if (argc > 1) {
        DataManager::load(std::string(argv[1]));
        std::cout << "DataManager load done." << std::endl;
//        std::cout << std::string(argv[2]) << std::endl;
        try {
            trainBoom->inside_load(std::string(argv[2]));
        }
        catch (const exception& e) {
            std::cout << e.what() << std::endl;
            throw;
        }
    }
    else {
        trainBoom->loadFromCSV("src/test/test.csv");
        trainBoom->loadModifications("src/test/FinalMushroom.out");
    }
    API::APIServer apiServer(trainBoom, 3000);
    apiServer.run(1);
}
