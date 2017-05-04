#include <fstream>
#include <cstdio>
#include <string>

const std::string BIN = "test_bin/";

int main(int argc, char **argv) {
    std::string object_name(argv[1]);

    system(("rm -f " + BIN + object_name + "_tests").c_str());
    if (object_name == "API") {
        system(("cd build/ && cmake -g \"Unix Makefiles\" .. &&\
                make -B " + object_name + "_tests").c_str());;
        system(("mv build/" + object_name + "_tests " + BIN).c_str());
    }
    else {
        system(("make -B test_" + object_name).c_str());
    }
    system(("./" + BIN + object_name + "_tests").c_str());
}
