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
    stupid_array<stupid_ptr<int>> a(new stupid_ptr<int>[5], 5);
    for (int i = 0; i < 5; ++ i) a[i] = make_stupid<int>(i);

    IntervalManip<int, int, ModifierAdd, MergerTAdd, MergerM>
        intervalManipAdd(a, 5);

    intervalManipAdd.forceApply();
    for (int i = 0; i < 5; ++ i) {
        assert(*(a[i]) == i);
    }
    std::cout << "Initialization test passed!" << std::endl;
    
    try {
        intervalManipAdd.query(0, 5);
        std::cout << "throw test for query out of range failed!" << std::endl;
        assert(false);
    } catch (const index_out_of_range& e) {
        std::cout << "throw test for query out of range passed!" << std::endl;
    }

    try {
        intervalManipAdd.modify(0, 5, 3);
        std::cout << "throw test for modify out of range failed!" << std::endl;
        assert(false);
    } catch (const index_out_of_range& e) {
        std::cout << "throw test for modify out of range passed!" << std::endl;
    }

    // std::cout << "querying [2, 3]: " << intervalManipAdd.query(2, 3) << std::endl;  // Should be 5
    assert(intervalManipAdd.query(2, 3) == 5);
    // std::cout << "querying [0, 4]: " << intervalManipAdd.query(0, 4) << std::endl;  // Should be 10
    assert(intervalManipAdd.query(0, 4) == 10);
    std::cout << "query test passed!" << std::endl;

    // std::cout << "now [0, 2] add -3 -> [-3, -2, -1, 3, 4]" << std::endl;
    intervalManipAdd.modify(0, 2, -3);
    // std::cout << "querying [2, 3]: " << intervalManipAdd.query(2, 3) << std::endl;  // Should be 2
    assert(intervalManipAdd.query(2, 3) == 2);
    // std::cout << "querying [0, 4]: " << intervalManipAdd.query(0, 4) << std::endl;  // Should be 1
    assert(intervalManipAdd.query(0, 4) == 1);
    std::cout << "query after modify test passed!" << std::endl;

    intervalManipAdd.forceApply();
    assert(*(a[0]) == -3);
    assert(*(a[1]) == -2);
    assert(*(a[2]) == -1);
    assert(*(a[3]) == 3);
    assert(*(a[4]) == 4);
    std::cout << "modify test passed!" << std::endl;
    
    std::cout << "all test passed!" << std::endl;
}
