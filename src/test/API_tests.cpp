#include "API.hpp"
#include "TrainBoom.hpp"
#include "util.hpp"

using namespace TrainBoom;

int main() {
    API::APIServer apiServer(util::make_stupid<TrainBoom::TrainBoom>(), 3000);
    apiServer.run(1);
//    apiServer.shutdown();
}
