#ifndef TRAINBOOM_UTIL_JSON_HPP
#define TRAINBOOM_UTIL_JSON_HPP

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <functional>
#include "util/BinaryFile.hpp"
#include "util/stupid_ptr.hpp"
// #include "util.hpp"

namespace trainBoom {

namespace util {

namespace json {

using namespace rapidjson;

typedef enum {
    BIN_TYPE_CLOSURE                = 0x01,
    BIN_TYPE_LIST                   = 0x02,
    BIN_TYPE_DICT                   = 0x03,
    BIN_TYPE_BOOL                   = 0x04,     /* 0000 0100 T */
    BIN_TYPE_BOOL_FALSE             = 0x05,     /* 0000 0101 F */

    BIN_TYPE_FLOAT_DOUBLE           = 0x06,     /* 0010 0110   */
    BIN_TYPE_FLOAT_SINGLE           = 0x07,     /* 0000 0111   */

    BIN_TYPE_NULL                   = 0x0f,

    BIN_TYPE_BLOB                   = 0x10,     /* 0001 xxxx   */
    BIN_TYPE_STRING                 = 0x20,     /* 0010 xxxx   */

    BIN_TYPE_INTEGER                = 0x40,     /* 010x xxxx + */
    BIN_TYPE_INTEGER_NEGATIVE       = 0x60,     /* 011x xxxx - */
} bin_type_t;

class Json {
public:
    class type_error : public exception {
    public:
    	type_error(const std::string& type) : exception(
    		"type_error",
    		"It is not a " + type + "!!!") {}
    };

    class JsonValue {
        friend class Json;
        Document::AllocatorType* allocator;
        Value *value;
    public:
        JsonValue(): allocator(nullptr), value(nullptr) {}
        JsonValue(Document::AllocatorType* allocator): allocator(allocator) {}
        JsonValue(Document::AllocatorType* allocator,
            Value *value): allocator(allocator), value(value) {}

        void write(util::stupid_ptr<BinaryFile> bfp) const {
            BinaryFile& bf = *bfp;
            if (value->IsBool()) {
                bool tmp = value->GetBool();
                bf.Write(BIN_TYPE_BOOL);
                bf.Write(tmp);
            }
            else if (value->IsInt()) {
                int tmp = value->GetInt();
                bf.Write(BIN_TYPE_INTEGER);
                bf.Write(tmp);
            }
            else if (value->IsString()) {
                std::string tmp = value->GetString();
                bf.Write(BIN_TYPE_STRING);
                bf.Write(tmp);
            }
            else if (value->IsDouble()) {
                double tmp = value->GetDouble();
                bf.Write(BIN_TYPE_FLOAT_DOUBLE);
                bf.Write(tmp);
            }
            else if (value->IsArray()) {
                bf.Write(BIN_TYPE_LIST);
                int size = value->Size();
                bf.Write(size);
                forEach([bfp, &bf](JsonValue jv) {
                    jv.write(bfp);
                });
            }
            else if (value->IsObject()) {
                bf.Write(BIN_TYPE_DICT);
                // std::cout << BIN_TYPE_DICT << std::endl;
                // bin_type_t tmp;

                int size = value->MemberCount();
                bf.Write(size);
                forEach([bfp, &bf](const std::string& key, JsonValue jv) {
                    bf.Write(key);
                    jv.write(bfp);
                });
                // bf.Close();
                // bf.Read(tmp);
                // std::cout << tmp << std::endl;
            }
            else throw type_error("writable JsonValue");
        }

        JsonValue& read(util::stupid_ptr<BinaryFile> bfp) {
            // std::cout << "JsonValue read {" << std::endl;
            BinaryFile& bf = *bfp;
            bin_type_t type; bf.Read(type);
            // std::cout << type << std::endl;
            if (type == BIN_TYPE_BOOL) {
                bool tmp; bf.Read(tmp);
                value->SetBool(tmp);
            }
            else if (type == BIN_TYPE_INTEGER) {
                int tmp; bf.Read(tmp);
                value->SetInt(tmp);
            }
            else if (type == BIN_TYPE_STRING) {
                std::string tmp; bf.Read(tmp);
                value->SetString(tmp, *allocator);
            }
            else if (type == BIN_TYPE_FLOAT_DOUBLE) {
                double tmp; bf.Read(tmp);
                value->SetDouble(tmp);
            }
            else if (type == BIN_TYPE_LIST) {
                int size; bf.Read(size);
                value->SetArray();
                for (int i = 0; i < size; ++ i) {
                    Value tmp;
                    value->PushBack(JsonValue(allocator, &tmp).read(bfp).getValue(), *allocator);
                }
            }
            else if (type == BIN_TYPE_DICT) {
                int size; bf.Read(size);
                value->SetObject();
                for (int i = 0; i < size; ++ i) {
                    Value tmp;
                    std::string key;
                    bf.Read(key);
                    value->AddMember(Value(key, *allocator), JsonValue(allocator, &tmp).read(bfp).getValue(), *allocator);
                }
            }
            else throw type_error("readable JsonValue");
            // std::cout << "JsonValue read }" << std::endl;
            return *this;
        }

        const Value& getValue() const {
            return *value;
        }

        Value& getValue() {
            return *value;
        }

        operator size_t() const {
            /*if (!value->IsUint64())
                throw type_error("size_t");
            return value->GetUint64();*/
            return int(*this);
        }

        operator unsigned() const {
            /*if (!value->IsUint64())
                throw type_error("unsigned");
            return value->GetUint64();*/
            return int(*this);
        }

        /*operator Id() const {
            return value->GetString();
        }*/

        operator std::string() const {
            if (!value->IsString())
                throw type_error("string");
            return value->GetString();
        }

        operator int() const {
            if (!value->IsInt())
                throw type_error("int");
            return value->GetInt();
        }

        operator double() const {
            if (!value->IsDouble())
                throw type_error("double");
            return value->GetDouble();
        }

        operator bool() const {
            if (!value->IsBool())
                throw type_error("bool");
            return value->GetBool();
        }

        template <typename T>
            T as() const {
                return T(*this);
            }

        JsonValue& operator=(const size_t& x) {
            value->SetInt(x);
            return *this;
        }

        JsonValue& operator=(const unsigned& x) {
            value->SetInt(x);
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
/*
        template <class T>
        JsonValue& operator=(const T& x) {
            return this->operator=(x.toJson());
        }
*/
        JsonValue& operator=(Json x) {
            value->SetObject();
            value->CopyFrom(x.document, *allocator);
            return *this;
        }

        JsonValue operator[](std::string key) {
            if (!value->IsObject())
                throw type_error("object");
            if (!value->HasMember(key)) {
                value->AddMember(Value(key, *allocator), Value(), *allocator);
            }
            return JsonValue(allocator, &((*value)[key]));
        }

        JsonValue operator[](unsigned int pos) {
            if (!value->IsArray())
                throw type_error("array");
            return JsonValue(allocator, &((*value)[pos]));
        }

        JsonValue operator[](std::string key) const {
            if (!value->IsObject())
                throw type_error("object");
            return JsonValue(allocator, &((*value)[key]));
        }

        JsonValue operator[](unsigned int pos) const {
            if (!value->IsArray())
                throw type_error("array");
            return JsonValue(allocator, &((*value)[pos]));
        }

        JsonValue& PushBack(const std::string& x) {
            if (!value->IsArray())
                throw type_error("array");
            value->PushBack(Value().SetString(x, *allocator), *allocator);
            return *this;
        }

        JsonValue& PushBack(const int& x) {
            if (!value->IsArray())
                throw type_error("array");
            value->PushBack(Value().SetInt(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const double& x) {
            if (!value->IsArray())
                throw type_error("array");
            value->PushBack(Value().SetDouble(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const bool& x) {
            if (!value->IsArray())
                throw type_error("array");
            value->PushBack(Value().SetBool(x), *allocator);
            return *this;
        }

        JsonValue& PushBack(const Json& x) {
            if (!value->IsArray())
                throw type_error("array");
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
            if (!value->IsArray())
                throw type_error("array");
            return value->Size();
        }

        bool HasMember(const std::string& key) const {
            if (!value->IsObject())
                throw type_error("object");
            return value->HasMember(key);
        }

        void forEach(std::function< void (const std::string&, JsonValue) > func) const {
            if (!value->IsObject())
                throw type_error("object");
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

        void forEach(std::function< void (JsonValue) > func) const {
            if (!value->IsArray())
                throw type_error("array");
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
        // document.AddMember("data", Value(kObjectType), *allocator);
        data = JsonValue(
            allocator,
            &(document)
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
        if (jv.HasMember("data")) {
            document.CopyFrom(jv.getValue(), *allocator);
            if (document.HasMember("type"))
                type = document["type"].GetString();
            if (document.HasMember("id"))
                id = document["id"].GetString();
            data = JsonValue(allocator, &document["data"]);
        }
        else {
            document.SetObject();
            document.AddMember("data", Value(kObjectType), *allocator);
            data = JsonValue(
                    allocator,
                    &(document["data"])
                    );
            document["data"].CopyFrom(jv.getValue(), *allocator);
        }
    }

    void write(util::stupid_ptr<BinaryFile> bfp) const {
        // std::cerr << "Json {" << std::endl;
        data.write(bfp);
        // std::cout << "Json }" << std::endl;
    }

    Json& read(util::stupid_ptr<BinaryFile> bfp) {
        // std::cerr << "???" << std::endl;
        data.read(bfp);
        return *this;
    }

    Json& read(std::string _id, stupid_ptr<BinaryFile> bfp) {
        // try {
            id = _id; data.read(bfp);
            return *this;
        // }
        // catch (const exception& e) {
            // std::cerr << e.what() << std::endl;
            // throw e;
        // }
    }

    Json& Parse(const std::string& content) {
//        std::cout << content << std::endl;
//        document["data"].Swap(Document().Parse(content));
//        document["data"] = Document().Parse(content);
//        data = Document().Parse(content);
        data.getValue().CopyFrom(Document().Parse(content), *allocator);
//        std::cout << this->toString() << std::endl;
        return *this;
    }

    std::string getType() const {
        return type;
    }

    std::string getId() const {
        return id;
    }

    void setId(std::string _id) {
        id = _id;
    }

    const Document& getDocument() const {
        return document;
    }

    JsonValue operator[](std::string key) {
        return data[key];
    }

    JsonValue operator[](unsigned int pos) {
        return data[pos];
    }

    JsonValue operator[](std::string key) const {
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
