#include "DataManager.hpp"
#include "Blob.hpp"
#include "util/map.hpp"
using namespace TrainBoom;

int main() {
    DataManager::init();

    Blob blob("123124");
    blob.save();

    Blob newblob(blob.id, DataManager::getFile(blob.id));
    // std::cout << newblob.content << std::endl;
    assert(newblob.content == blob.content);
    std::cout << "Blob save & load test passed!" << std::endl;

    util::map<std::string, Blob> testMap;
    for (int i = 0; i < 1000; ++ i) {
        testMap.insert(util::make_pair(IdNamespace::getSalt(10), Blob(IdNamespace::getSalt(10))));
    }

    testMap.save();

    util::map<std::string, Blob> newTestMap(testMap.getId(), DataManager::getFile(testMap.getId()));
    unsigned cnt = 0;
    for (const auto& item: newTestMap) {
        // std::cout << item.first << " " << std::string(item.second) << std::endl;
        assert(std::string(testMap[item.first]) == std::string(item.second));
        ++ cnt;
    }
    assert(cnt == testMap.size());
    std::cout << "content consistency test passed!" << std::endl;
    // return 0;
    for (int i = 0; i < 1000; ++ i) {
        // std::cout << i << std::endl;
        std::string key = IdNamespace::getSalt(10), value = IdNamespace::getSalt(10);
        // std::cout << key << " " << value << std::endl;
        newTestMap.insert(util::make_pair(key, Blob(value)));
        assert(std::string(newTestMap[key]) == value);
    }
    assert(newTestMap.size() == 2000);

    std::cout << "accessiblity test after load passed!" << std::endl;
}
