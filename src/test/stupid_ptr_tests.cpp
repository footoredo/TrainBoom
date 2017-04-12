#include "util/stupid_ptr.hpp"
#include <iostream>

int main() {
    using util::stupid_ptr;
    stupid_ptr<int> x(new int(5));
    std::cout << *x << std::endl;
    stupid_ptr<int, util::array_deleter<int>> a(new int[5]);
    for (int i = 0; i < 5; ++ i) {
        a[i] = i;
        std::cout << a[i] << std::endl;
    }
}
