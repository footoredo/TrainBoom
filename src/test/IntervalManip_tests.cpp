#include <iostream>
#include "util/IntervalManip.hpp"
#include "util/stupid_ptr.hpp"

using namespace TrainBoom;
using namespace util;

struct ModifierAdd {
    int operator()(int x, int y, size_t l, size_t r) {
        return x + y * (r - l + 1);
    }
};

struct ModiferMax {
    int operator()(int x, int y, size_t l, size_t r) {
        return x + y;
    }
};

struct MergerTAdd {
    int operator()(int x, int y) {
        return x + y;
    }
};

struct MergerM {
    int operator()(int x, int y) {
        return x + y;
    }
};

int main() {
    stupid_array<int> a(new int[5], 5);
    for (int i = 0; i < 5; ++ i) a[i] = i;

    IntervalManip<int, int, ModifierAdd, MergerTAdd, MergerM>
        intervalManipAdd(a, 5);

    intervalManipAdd.forceApply();
    for (int i = 0; i < 5; ++ i)
        std::cout << a[i] << " ";
    std::cout << std::endl;
    
    try {
        std::cout << intervalManipAdd.query(0, 5) << std::endl;;
    } catch (const index_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    try {
        intervalManipAdd.modify(0, 5, 3);
    } catch (const index_out_of_range& e) {
        std::cout << "ok!" << std::endl;
    }

    std::cout << "querying [2, 3]: " << intervalManipAdd.query(2, 3) << std::endl;  // Should be 5
    std::cout << "querying [0, 4]: " << intervalManipAdd.query(0, 4) << std::endl;  // Should be 10

    std::cout << "now [0, 2] add -3 -> [-3, -2, -1, 3, 4]" << std::endl;
    intervalManipAdd.modify(0, 2, -3);
    std::cout << "querying [2, 3]: " << intervalManipAdd.query(2, 3) << std::endl;  // Should be 2
    std::cout << "querying [0, 4]: " << intervalManipAdd.query(0, 4) << std::endl;  // Should be 1


}
