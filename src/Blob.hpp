#ifndef TRAINBOOM_BLOB_HPP
#define TRAINBOOM_BLOB_HPP

#include "util/Json.hpp"
#include "util/stupid_ptr.hpp"
#include "Id.hpp"
#include "DataManager.hpp"
#include <iostream>

namespace trainBoom {
    struct Blob {
        std::string content;
        std::string id;
        Blob(): content(), id(Id("Blob")) {}
        Blob(std::string content): content(content), id(Id("Blob")) {}
        Blob(const Json& json) {
            // std::cout << "!" << std::endl;
            // std::cout << json.toString() << std::endl;
            if (json.getId() == "") {
                id = Id("Blob");
            }
            else {
                id = json.getId();
            }
            content = json["content"].as<std::string>();
        }
        static Blob load(std::string id) {
            return Blob(DataManager::getJson(id));
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
        	DataManager::save(toJson());
        }
        bool operator<(const Blob& other) const {
            return content < other.content;
        }
        friend std::ostream& operator<<(std::ostream &fout, const Blob& blob) {
			fout << blob.content;
			return fout;
		}
        friend std::istream& operator>>(std::istream &fin, Blob& blob) {
			fin >> blob.content;
			return fin;
		}
    };
}

#endif
