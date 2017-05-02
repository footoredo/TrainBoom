#include <fstream>
#include <cstdio>
#include <string>

int main(int argc, char **argv) {
    std::string object_name(argv[1]);

    std::ofstream Makefile;
    Makefile.open("Makefile", std::ios::out | std::ios::app);

    Makefile << std::endl
        << "test_" + object_name + ":" << std::endl
        << "\t$(CXX) -o $(BIN)/" + object_name + "_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/" + object_name + "_tests.cpp" << std::endl;
}
