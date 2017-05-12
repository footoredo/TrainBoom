#include "DataManager2.hpp"
#include "Blob.hpp"
// #include "util/map.hpp"
// #include "util/set.hpp"
using namespace trainBoom;

int main() {
    DataManager::init();
    // std::cout << "done" << std::endl;

    Blob blob("123124");
    // std::cout << "done" << std::endl;
    blob.save();
    std::cout << "save done." << std::endl;
    std::string key = DataManager::finish();

    try {
        DataManager::load(key);
    }
    catch (const exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "load done." << std::endl;

    Blob newblob = Blob::load(blob.getId());
    // std::cout << newblob.content << std::endl;
    assert(newblob.content == blob.content);
    std::cout << "Blob save & load test passed!" << std::endl;

}
