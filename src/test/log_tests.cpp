#include "util/log.hpp"
#include <iostream>
using namespace trainBoom;

int main() {
    Log log("test");
    log.log("lalalalala");
    log.log("中文中文");
    log.log("");

    auto all = log.get();

    for (const auto& msg: all) {
        std::cout << msg << std::endl;
    }
}
