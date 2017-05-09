#ifndef TRAINBOOM_DATAMANAGER_HPP
#define TRAINBOOM_DATAMANAGER_HPP

#include "util/BinaryFile.hpp"
#include "util/stupid_ptr.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

namespace TrainBoom {
    namespace DataManager {
        const char root[] = ".TrainBoom/";
        const int root_len = strlen(root);

        void touch_dir(const char* dir) {
            if (!~access(dir, 0)) {
                mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            }
        }

        void init() {
            touch_dir(root);
        }

        util::stupid_ptr<BinaryFile> getFile(std::string id) {
            char file_path[root_len + 41 + 1];
            strcpy(file_path, root);
            strncpy(file_path + root_len, id.c_str(), 2);
            file_path[root_len + 2] = '/';
            file_path[root_len + 3] = '\0';
            touch_dir(file_path);
            strcpy(file_path + root_len + 3, id.c_str() + 2);
//            std::cout << file_path << std::endl;

            return util::make_stupid<BinaryFile>(std::string(file_path));
        }
    }
}

#endif
