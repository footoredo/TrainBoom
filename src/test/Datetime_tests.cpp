#include "util/Datetime.hpp"
#include <iostream>

using namespace TrainBoom;
using namespace util::Datetime;

int main() {
    try {
        Datetime datetime(-2017, 4, 10, 22, 57);
    } catch (const time_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    try {
        Datetime datetime(0, 1, 1, 0, -1);
    } catch (const time_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    try {
        Duration duration(0, 0, -1);
    } catch (const time_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    Duration duration(3, 25, -6);
    std::cout << std::string(duration) << std::endl;

    Datetime datetime(2017, 4, 10, 25, -7);
    std::cout << std::string(datetime) << std::endl;

    std::cout << std::string(datetime + duration) << std::endl;

    std::cout << std::string(datetime - duration) << std::endl;

    std::cout << std::string(duration - duration) << std::endl;

    std::cout << std::string(Datetime::parse("1015/12/6 7:33")) << std::endl;

    std::cout << Datetime::parse("1015/12/6 7:33") << std::endl;

}
