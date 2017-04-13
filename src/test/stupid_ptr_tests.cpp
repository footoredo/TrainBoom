#include "util/stupid_ptr.hpp"
#include <iostream>

int main() {
    using util::stupid_ptr;
    using util::stupid_array;
    stupid_ptr<int> x(new int(5)), y(x);
    std::cout << *x << std::endl;
    std::cout << *y << std::endl;
    // x.dec();
    stupid_array<int> a(new int[5], 5), b(a);
    for (int i = 0; i < 5; ++ i) {
        a[i] = i;
        std::cout << a[i] << std::endl;
    }
    for (int i = 0; i < 5; ++ i) {
        std::cout << b[i] << std::endl;
    }
    // a.dec(); b.dec();
}
