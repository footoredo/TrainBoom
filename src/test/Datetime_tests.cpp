#include "util/Datetime.hpp"
#include <iostream>

using namespace trainBoom;
using namespace util::date_time;

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
        Duration duration(0, -1);
    } catch (const time_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    Duration duration(3* 24 + 25, -6);
    std::cout << std::string(duration) << std::endl;

    Datetime datetime(2017, 4, 10, 25, -7);
    std::cout << std::string(datetime) << std::endl;

    std::cout << std::string(datetime + duration) << std::endl;

    std::cout << std::string(datetime - duration) << std::endl;

    std::cout << std::string(duration - duration) << std::endl;

    std::cout << std::string(Datetime::parse("1015/12/6 7:33")) << std::endl;

    std::cout << Datetime::parse("1015/12/6 7:33") << std::endl;

    std::cout << Datetime::now() << std::endl;

    std::cout << Datetime("1015/12/6 7:33") << std::endl;

    std::cout << Datetime("1015/12/31").incDay() << std::endl;

    std::cout << Datetime("2017/5/13 00:15") - Datetime("2017/4/20 00:30") << std::endl;

    std::cout << Datetime("2017/5/5").incDay() << std::endl;

    try {
        std::cout << Datetime("2017/5/13 00:15") - Datetime("2018/4/20 00:30") << std::endl;
        std::cout << "negative duration test failed!" << std::endl;
    }
    catch (const duration_is_negative& e) {
        std::cout << "negative duration test passed!" << std::endl;
    }

    std::cout << Duration("24:55") << std::endl;

    std::cout << Datetime("2017-5-5") << std::endl;
}
