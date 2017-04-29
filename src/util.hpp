#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <string>

namespace TrainBoom {

typedef std::string Id;
using TrainBoom::Id;
#define TESTTESTTEST
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

}

#include "util/CSV.hpp"
#include "util/Datetime.hpp"
#include "util/IntervalManip.hpp"
#include "util/map.hpp"
#include "util/pair.hpp"
#include "util/stupid_ptr.hpp"
/*#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"*/
#include "util/Json.hpp"

#endif
