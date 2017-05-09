#ifndef TRAINBOOM_BLOB_HPP
#define TRAINBOOM_BLOB_HPP

#include "util/Json.hpp"
#include "util/stupid_ptr.hpp"
#include "Id.hpp"
#include "DataManager.hpp"

namespace TrainBoom {
    struct Blob {
        std::string content;
        std::string id;
        Blob(): content(), id(Id("Blob")) {}
        Blob(std::string content): content(content), id(Id("Blob")) {}
        Blob(std::string id, util::stupid_ptr<BinaryFile> bfp): id(id) {
            Json tmp; tmp.read(bfp);
            content = tmp["content"].as<std::string>();
        }
        operator std::string() const {
            return content;
        }
        std::string getId() const {
            return id;
        }
        Json toJson() const {
            Json json("blob", id);
            json["content"] = content;
            return json;
        }
        void save() const {
        	toJson().write(DataManager::getFile(id));
        }
    };
}

#endif
