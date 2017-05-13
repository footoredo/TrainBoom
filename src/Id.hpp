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
            for (unsigned i = 0; i < length; ++ i) {
                int key = rand() % (26 * 2 + 10);
                char ch;
                if (key < 10) ch = key + '0';
                else if ((key -= 10) < 26) ch = key + 'a';
                else ch = key - 26 + 'A';
                salt += ch;
            }
            return salt;
        }

        std::string sha1(std::string input) {
            SHA1 checksum;
            checksum.update(input);
            return checksum.final();
        }

        std::string encrypt(std::string password, std::string salt) {
            return sha1(password + "|" + salt);
        }

        // typedef std::string Id;

        std::string generateId(const std::string& type, time_t createTime) {
/*            std::stringstream ss;
            ss << type << " " << createTime << " " << getSalt(8);
            SHA1 sha; sha.update(ss.str());
            // std::cout << "--- generating sha1 of ---\n";
            // std::cout << ss.str() << std::endl;
            // std::cout << "--- end ---\n";*/
//            return sha.final();
            return getSalt(8);
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
    using IdNamespace::sha1;
    using IdNamespace::encrypt;
}

#endif
