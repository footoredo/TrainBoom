#include "util/stupid_ptr.hpp"
#include <iostream>

int main() {
    using util::stupid_ptr;
    stupid_ptr<int> x(new int(5));
    std::cout << *x << std::endl;
}
