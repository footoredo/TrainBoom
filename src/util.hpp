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

const double eps = 1e-8;
double abs(double x) {
    if (x < 0) return -x;
    else return x;
}
bool equal(double a, double b) {
    return abs(a - b) < eps;
}

#include "util/Json.hpp"
#include "util/pair.hpp"
#include "util/stupid_ptr.hpp"
#include "util/BinaryFile.hpp"
#include "Id.hpp"
#include "DataManager.hpp"
#include "Blob.hpp"

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

#endif
