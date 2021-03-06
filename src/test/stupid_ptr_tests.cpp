#include "util/stupid_ptr.hpp"
#include <iostream>
#include <cassert>
using namespace trainBoom;

int main() {
    using util::stupid_ptr;
    using util::stupid_array;

    auto tmp = util::make_stupid<std::pair<int, int>>(5, 6);
    assert(tmp->first == 5 && tmp->second == 6);

    stupid_ptr<int> x(new int(5)), y(x);
    assert(*x == 5 && *y == 5);
    // x.dec();
    stupid_array<int> a(new int[5], 5), b(a);
    for (int i = 0; i < 5; ++ i) {
        a[i] = i;
//        std::cout << a[i] << std::endl;
        assert(a[i] == i);
    }
    for (int i = 0; i < 5; ++ i) {
//        std::cout << b[i] << std::endl;
        assert(b[i] == i);
    }
    for (int i = 0; i < 5; ++ i) {
        a[i] = 5 - i;
        assert(a[i] == 5 - i);
    }
    a.sort();
    int t = 1;
    for (int x: a) {
        assert(x == t);
        ++ t;
    }
    std::cout << "All tests passed!" << std::endl;
    // a.dec(); b.dec();
}
