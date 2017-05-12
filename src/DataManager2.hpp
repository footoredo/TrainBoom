#ifndef TRAINBOOM_DATAMANAGER2_HPP
#define TRAINBOOM_DATAMANAGER2_HPP

#include "DataManager.hpp"
#include "util/map.hpp"
#include "util/Json.hpp"
#include "util/stupid_ptr.hpp"

namespace trainBoom {

namespace DataManager {

class object_has_no_id : public exception{
public:
    object_has_no_id(const Json& json):
        exception("object_has_no_id","The object [" + json.toString() + "] you saved has no id!") {};
};

const int MAX_OBJECT_COUNT = 5e6;
util::map<std::string, unsigned> json_map;
util::stupid_ptr<Json> json_objects[MAX_OBJECT_COUNT];

void init() {
    srand(time(nullptr) ^ getpid());
    touch_dir(root.c_str());
    data_file_name = getSalt(10);
    data_file_p = make_stupid<BinaryFile>(root + data_file_name);
    json_map.clear();
}

Json getJson(std::string id) {
    try {
        // std::cout << json_map.at(id).getId() << std::endl;
        auto iter = json_map.find(id);
        unsigned index = iter->second;
        Json json = *json_objects[index];
        json_map.erase(iter);
        json_objects[index] = nullptr;
        return json;
    }
    catch (const exception& e) {
        throw id_not_found(id);
    }
}

void load(std::string last_data_file_name) {
    stupid_ptr<BinaryFile> last_data_file_p = make_stupid<BinaryFile>(root + last_data_file_name);
    int cnt = 0;
    while (true) {
        bool obj; last_data_file_p->Read(obj);
        if (!obj) break;
        else {
            std::string id; last_data_file_p->Read(id);
            // std::cout << id << std::endl;
            // Json tmp; tmp.read(id, last_data_file_p);
            // std::cout << "done" << std::endl;
            json_objects[cnt] = make_stupid<Json>();
            json_objects[cnt]->read(id, last_data_file_p);
            json_map.insert(util::make_pair(id, cnt));
            ++ cnt;
            // if (cnt % 1000 == 0)
                // std::cout << cnt + " objects have been loaded." << std::endl;
        }
    }
    std::cout << "A total of " << cnt << " object(s) have been loaded." << std::endl;
    // std::cout << json_map.size() << std::endl;
}

std::string finish() {
    data_file_p->Write(false);
    data_file_p = nullptr;
    return data_file_name;
}

void save(const Json& json) {
    data_file_p->Write(true);
    if (json.getId() != "") {
        data_file_p->Write(json.getId());
        json.write(data_file_p);
    }
    else {
        throw object_has_no_id(json);
    }
}

}

}

#endif
