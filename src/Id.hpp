#ifndef TRAINBOOM_ID_HPP
#define TRAINBOOM_ID_HPP

#include <ctime>
#include <iostream>
#include "sha1.hpp"
#include <sstream>

namespace trainBoom {
    namespace IdNamespace {
        std::string getSalt(unsigned length) {
            std::string salt;
            for (unsigned i = 0; i < length; ++ i)
                salt += rand() % 26 + 'a';
            return salt;
        }

        // typedef std::string Id;

        std::string generateId(const std::string& type, time_t createTime) {
            std::stringstream ss;
            ss << type << " " << createTime << " " << getSalt(8);
            SHA1 sha; sha.update(ss.str());
            // std::cout << "--- generating sha1 of ---\n";
            // std::cout << ss.str() << std::endl;
            // std::cout << "--- end ---\n";
            return sha.final();
        }

        class Id {
        private:
            time_t createTime;
            std::string content;
        public:
            Id(std::string type): createTime(std::time(nullptr)), content(generateId(type, createTime)) {
                // std::cout << "new id " << content << std::endl;
            }
            // Id(const Id& other): createTime(createTime), content(content) {}
            operator std::string() const {
                return content;
            }
            bool operator<(const Id& other) const {
                return content < other.content;
            }
            bool operator==(const Id& other) const {
                return content == other.content;
            }
            friend std::ostream& operator<<(std::ostream &fout, const Id& id) {
                fout << id.content;
                return fout;
            }
        };
    }
    using IdNamespace::Id;
    using IdNamespace::getSalt;
}

#endif
