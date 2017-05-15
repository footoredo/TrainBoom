#ifndef TRAINBOOM_UTIL_LOG_HPP
#define TRAINBOOM_UTIL_LOG_HPP

#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <iomanip>
#include "util/vector.hpp"

namespace trainBoom {
    namespace util {
        namespace log_ns {
            // trim from start
            static inline std::string ltrim(std::string s) {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))));
                return s;
            }
            // trim from end
            static inline std::string rtrim(std::string s) {
                s.erase(std::find_if(s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
                return s;
            }
            // trim from both ends
            static inline std::string trim(std::string s) {
                return ltrim(rtrim(s));
            }

            const std::string root = ".log/";

            inline void put_now(std::ofstream& fout) {
                time_t now = std::time(nullptr);
                std::tm tm = *std::localtime(&now);
                fout << std::put_time(&tm, "<%F %T %Z> ") << std::flush;
            }

            void touch_dir(const char* dir) {
                if (!~access(dir, 0)) {
                    mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                }
            }

            class Log {
            private:
                std::string file_name;
                std::ofstream log_file;

            public:
                Log(std::string type) {
                    file_name = root + type + ".log";
                    touch_dir(root.c_str());
                    log_file.open(file_name, std::ofstream::out | std::ofstream::app);
                    // log_file.imbue(std::locale("ja_JP"));
                }

                ~Log() {
                    log_file.close();
                }

                void log(std::string msg) {
                    put_now(log_file);
                    log_file << msg << std::endl << std::flush;
                }

                util::vector<std::string> get() const {
                    std::ifstream input_log_file(file_name);
                    util::vector<std::string> ret;
                    std::string line;
                    while (std::getline(input_log_file, line)) {
                        ret.push_back(trim(line));
                    }
                    return ret;
                }
            };
        }
    }

    using util::log_ns::Log;
}

#endif
