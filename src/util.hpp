#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include "sha1.hpp"

namespace TrainBoom {


/*class Id {
private:
    std::string content;
    SHA1 sha;
public:
    Id() {}
    Id(const std::string& str) {
        sha.update(str);
    }

    void update(const std::string& str) {
        sha.update(str);
        content = "";
    }

    operator std::string() {
        if (content.size()) {
            return content;
        }
        else {
            return content = sha.final();
        }
    }
};
*/

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
    Id(const std::string& type): createTime(std::time(nullptr)), content(generateId(type, createTime)) {
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

using TrainBoom::Id;

const double eps = 1e-8;
double abs(double x) {
    if (x < 0) return -x;
    else return x;
}
bool equal(double a, double b) {
    return abs(a - b) < eps;
}

// #define TESTTESTTEST
// using namespace rapidjson;

/*
void showJson(Document& document) {
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    document.Accept(writer);
	std::cout << sb.GetString() << std::endl;
}

Value& initiateDocument(Document& document, std::string name) {
    document.SetObject();
    document.AddMember("name", name, document.GetAllocator());
    Value data(kObjectType);
    document.AddMember("data", data, document.GetAllocator());
    return document["data"];
}
*/

namespace util {
    using std::vector;
}

}

#include "util/CSV.hpp"
#include "util/Datetime.hpp"
#include "util/IntervalManip.hpp"
#include "util/map.hpp"
#include "util/set.hpp"
#include "util/pair.hpp"
#include "util/stupid_ptr.hpp"
#include "util/Json.hpp"

#endif
