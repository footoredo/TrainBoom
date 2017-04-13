#include <fstream>
#include <cstdio>
#include <string>

const std::string BIN = "test_bin/";

int main(int argc, char **argv) {
    std::string object_name(argv[1]);

    system(("./" + BIN + object_name + "_tests").c_str());
}
