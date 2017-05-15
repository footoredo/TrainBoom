#ifndef TRAINBOOM_DATAMANAGER_HPP
#define TRAINBOOM_DATAMANAGER_HPP

#include "util/BinaryFile.hpp"
#include "util/stupid_ptr.hpp"
#include "Id.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <ctime>

namespace trainBoom {
    namespace util {
        namespace json {
            class Json;
        }
    }
    namespace DataManager {
        class id_not_found : public exception{
        public:
            id_not_found(std::string id):
                exception("id_not_found","id [" + id + "] is not found!!!") {};
        };

        const std::string root = ".TrainBoom/";
        std::string data_file_name;
        stupid_ptr<BinaryFile> data_file_p;

        void touch_dir(const char* dir) {
            if (!~access(dir, 0)) {
                mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            }
        }

        void init();
        void load(std::string last_data_file_name);
        util::json::Json getJson(std::string id);
        std::string finish();
        void save(const util::json::Json& json);

        /*util::stupid_ptr<BinaryFile> getFile(std::string id) {
            // std::cout << "getFile {" << std::endl;
            char file_path[root_len + 41 + 1];
            strcpy(file_path, root);
            strncpy(file_path + root_len, id.c_str(), 2);
            file_path[root_len + 2] = '/';
            file_path[root_len + 3] = '\0';
            touch_dir(file_path);
            strcpy(file_path + root_len + 3, id.c_str() + 2);
//            std::cout << file_path << std::endl;

            // std::cout << "getFile }" << std::endl;
            return util::make_stupid<BinaryFile>(std::string(file_path));
        }*/

    }
}

#endif
