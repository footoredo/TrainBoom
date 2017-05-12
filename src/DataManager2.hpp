#ifndef TRAINBOOM_DATAMANAGER2_HPP
#define TRAINBOOM_DATAMANAGER2_HPP

#include "DataManager.hpp"
#include "util/map.hpp"
#include "util/Json.hpp"

namespace trainBoom {

namespace DataManager {

class object_has_no_id : public exception{
public:
    object_has_no_id(const Json& json):
        exception("object_has_no_id","The object [" + json.toString() + "] you saved has no id!") {};
};

util::map<std::string, Json> json_map;

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
        Json json = iter->second;
        json_map.erase(iter);
        return json;
    }
    catch (const exception& e) {
        throw id_not_found(id);
    }
}

void load(std::string last_data_file_name) {
    stupid_ptr<BinaryFile> last_data_file_p = make_stupid<BinaryFile>(root + last_data_file_name);
    while (true) {
        bool obj; last_data_file_p->Read(obj);
        if (!obj) break;
        else {
            std::string id; last_data_file_p->Read(id);
            // std::cout << id << std::endl;
            // Json tmp; tmp.read(id, last_data_file_p);
            // std::cout << "done" << std::endl;
            json_map.insert(util::make_pair(id, Json().read(id, last_data_file_p)));
        }
    }
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
