#include "API.hpp"
#include "TrainBoom.hpp"
#include "DataManager.hpp"
#include <cxxopts.hpp>

using namespace trainBoom;

int main(int argc, char **argv) {
    cxxopts::Options options("TrainBoom", "Trains boomed.");
    options.add_options()
        ("c,csv", "Initial CSV file", cxxopts::value<std::string>())
        ("m,modification", "Initial modification file", cxxopts::value<std::string>())
        ("k,key", "Data key", cxxopts::value<std::string>())
        ("i,id", "TrainBoom id", cxxopts::value<std::string>())
        ("p,port", "Server port", cxxopts::value<int>())
        ("h,help", "Print help")
        ;

    options.parse(argc, argv);

    if (options.count("help")) {
        std::cout << options.help({""}) << std::endl;
        exit(0);
    }

    DataManager::init();
    auto trainBoom = make_stupid<trainBoom::TrainBoom>();

    if (options.count("csv")) {
        std::cout << "Loading CSV..." << std::endl;
        trainBoom->loadFromCSV(options["csv"].as<std::string>());
        std::cout << "done. " << std::endl;
    }

    if (options.count("modification")) {
        std::cout << "Loading modifications..." << std::endl;
        trainBoom->loadModifications(options["modification"].as<std::string>());
        std::cout << "done. " << std::endl;
    }

    if (options.count("key")) {
        std::cout << "Loading data..." << std::endl;
        DataManager::load(options["key"].as<std::string>());
        std::cout << "done. " << std::endl;
    }

    if (options.count("id")) {
        std::cout << "Restoring data..." << std::endl;
        trainBoom->inside_load(options["id"].as<std::string>());
        std::cout << "done. " << std::endl;
    }

    int port = 3000;
    if (options.count("port")) {
        port = options["port"].as<int>();
    }

    API::APIServer apiServer(trainBoom, port);
    apiServer.run(1);
}
