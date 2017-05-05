#ifndef TRAINBOOM_UTIL_JSON_HPP
#define TRAINBOOM_UTIL_JSON_HPP

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <functional>
#include "util.hpp"

namespace TrainBoom {

namespace util {

namespace json {

using namespace rapidjson;

class Json {
public:
    class JsonValue {
        friend class Json;
        Document::AllocatorType* allocator;
        Value *value;
    public:
        JsonValue(): allocator(nullptr), value(nullptr) {}
        JsonValue(Document::AllocatorType* allocator,
            Value *value): allocator(allocator), value(value) {}

        const Value& getValue() const {
            return *value;
        }

        operator size_t() const {
            return value->GetUint64();
        }

        operator unsigned() const {
            return value->GetUint64();
        }

        /*operator Id() const {
            return value->GetString();
        }*/

        operator std::string() const {
            return value->GetString();
        }

        operator int() const {
            return value->GetInt();
        }

        operator double() const {
            return value->GetDouble();
        }

        operator bool() const {
            return value->GetBool();
        }

        template <typename T>
            T as() const {
                return T(*this);
            }

        JsonValue& operator=(const size_t& x) {
            value->SetUint64(x);
            return *this;
        }

        JsonValue& operator=(const unsigned& x) {
            value->SetUint64(x);
            return *this;
        }

        /*JsonValue& operator=(const Id& x) {
            value->SetString(x, *allocator);
            return *this;
        }*/

        JsonValue& operator=(const std::string& x) {
            value->SetString(x, *allocator);
            return *this;
        }

        JsonValue& operator=(const char* x) {
            value->SetString(x, *allocator);
            return *this;
        }

        JsonValue& operator=(const int& x) {
            value->SetInt(x);
            return *this;
        }

        JsonValue& operator=(const double& x) {
            value->SetDouble(x);
            return *this;
        }

        JsonValue& operator=(const bool& x) {
            value->SetBool(x);
            return *this;
        }

        JsonValue& operator=(const Json& x) {
            value->SetObject();
            value->CopyFrom(x.document, *allocator);
            return *this;
        }

        JsonValue operator[](const std::string& key) {
            if (!value->HasMember(key)) {
                value->AddMember(Value(key, *allocator), Value(), *allocator);
            }
            return JsonValue(allocator, &((*value)[key]));
        }

        JsonValue operator[](unsigned int pos) {
            return JsonValue(allocator, &((*value)[pos]));
        }

        JsonValue operator[](const std::string& key) const {
            return JsonValue(allocator, &((*value)[key]));
        }

        JsonValue operator[](unsigned int pos) const {
            return JsonValue(allocator, &((*value)[pos]));
        }

        JsonValue& PushBack(const std::string& x) {
            value->PushBack(Value().SetString(x, *allocator), *allocator);
            return *this;
        }

        JsonValue& PushBack(const int& x) {
            value->PushBack(Value().SetInt(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const double& x) {
            value->PushBack(Value().SetDouble(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const bool& x) {
            value->PushBack(Value().SetBool(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const Json& x) {
            value->PushBack(Value().SetObject().CopyFrom(x.document, *allocator), *allocator);
            return *this;
        }

        JsonValue& SetArray() {
            value->SetArray();
            // std::cout << "!!!" << std::endl;
            return *this;
        }

        JsonValue& SetObject() {
            value->SetObject();
            return *this;
        }

        size_t Size() const {
            return value->Size();
        }

        bool HasMember(const std::string& key) const {
            return value->HasMember(key);
        }

        void forEach(std::function< void (const std::string&, JsonValue) > func) {
            for (auto& item: value->GetObject()) {
                func(item.name.GetString(),
                    JsonValue(allocator, &item.value));
            }
        }

/*        void forEach(std::function< void (Id, JsonValue) > func) {
            for (auto& item: value->GetObject()) {
                func(item.name.GetUint64(),
                    JsonValue(allocator, &item.value));
            }
        }*/

        void forEach(std::function< void (JsonValue) > func) {
            for (auto& item: value->GetArray()) {
                func(JsonValue(allocator, &item));
            }
        }
    };

private:
    Document document;
    Document::AllocatorType* allocator;
    JsonValue data;
    std::string type;
    std::string id;

public:
    Json() {
        allocator = &(document.GetAllocator());
        document.SetObject();
        document.AddMember("data", Value(kObjectType), *allocator);
        data = JsonValue(
            allocator,
            &(document["data"])
        );
    }
    Json(std::string type): type(type) {
        allocator = &(document.GetAllocator());
        document.SetObject();
        document.AddMember("type", type, *allocator);
        document.AddMember("data", Value(kObjectType), *allocator);
        data = JsonValue(
            allocator,
            &(document["data"])
        );
    }

    Json(std::string type, std::string id): type(type), id(id) {
        allocator = &(document.GetAllocator());
        document.SetObject();
        document.AddMember("type", type, *allocator);
        document.AddMember("id", id, *allocator);
        document.AddMember("data", Value(kObjectType), *allocator);
        data = JsonValue(
            allocator,
            &(document["data"])
        );
    }

    Json(const Json& jv) {
        allocator = &(document.GetAllocator());
        document.CopyFrom(jv.getDocument(), *allocator);
        if (document.HasMember("type"))
            type = document["type"].GetString();
        if (document.HasMember("id"))
            id = document["id"].GetString();
        data = JsonValue(allocator, &document["data"]);
    }

    Json(const JsonValue& jv) {
        allocator = &(document.GetAllocator());
        document.SetObject();
        document.AddMember("data", Value(kObjectType), *allocator);
        data = JsonValue(
            allocator,
            &(document["data"])
        );
        document["data"].CopyFrom(jv.getValue(), *allocator);
    }

    Json& Parse(const std::string& content) {
//        std::cout << content << std::endl;
//        document["data"].Swap(Document().Parse(content));
//        document["data"] = Document().Parse(content);
        document["data"].CopyFrom(Document().Parse(content), *allocator);
//        std::cout << this->toString() << std::endl;
        return *this;
    }

    const std::string& getType() const {
        return type;
    }

    const Document& getDocument() const {
        return document;
    }

    JsonValue operator[](const std::string& key) {
        return data[key];
    }

    JsonValue operator[](unsigned int pos) {
        return data[pos];
    }

    JsonValue operator[](const std::string& key) const {
        return data[key];
    }

    JsonValue operator[](unsigned int pos) const {
        return data[pos];
    }

    bool HasMember(const std::string& key) const {
        return data.HasMember(key);
    }

    void SetArray() {
        data.SetArray();
    }

    void SetObject() {
        data.SetObject();
    }

    std::string toString() const {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        document.Accept(writer);
    	return sb.GetString();
    }
};

} // json

using json::Json;


} // util

using util::json::Json;

} // TrainBoom

#endif
