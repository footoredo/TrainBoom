#ifndef TRAINBOOM_TICKET_HPP
#define TRAINBOOM_TICKET_HPP
#include <cstring>
#include <string>
#include <iostream>
#include "exception.hpp"
#include "util/Json.hpp"
#include "util/pair.hpp"

namespace trainBoom {

namespace Ticket {

using std :: ostream;

typedef std::string Type;

class Attribute {
	public:
		double price; // 车票单价
		unsigned number;  // 车票余量
		bool nonstop;

		Attribute(): price(), number(), nonstop() { // 无参构造，类型为空
		}

		Attribute(double p, unsigned n, bool nonstop = false): price(p), number(n), nonstop(nonstop) {}

		Attribute(const Attribute &t): price(t.price), number(t.number), nonstop(t.nonstop) {
			// price = t.price;
			// number = t.number;
			// type = t.type;
		}

		Attribute(const util::Json& json) {
            if (json.HasMember("p")) 
                price = (double)(json["p"].as<int>()) / 2;
            else 
                price = json["price"].as<double>();
            if (json.HasMember("nu"))
                number = json["nu"].as<unsigned>();
            else if (json.HasMember("number"))
                number = json["number"].as<unsigned>();
            else 
                number = 2000;
            if (json.HasMember("no"))
                nonstop = json["no"].as<int>();
            else if (json.HasMember("nonstop"))
                nonstop = json["nonstop"].as<bool>();
            else
                nonstop = false;
		}

		~Attribute(){}

		Attribute &operator=(const Attribute &t){
			if(&t == this)
				return *this;
			price = t.price;
			number = t.number;
			nonstop = t.nonstop;
			// type = t.type;
			return *this;
		}

		bool operator<(const Attribute& other) {
			if (price == other.price)
				if (number == other.number)
					return nonstop < other.nonstop;
				else
					return number < other.number;
			else
				return price < other.price;
		}

		friend ostream& operator<<(ostream &fout, const Attribute &rhs){ //测试用，重载输出
			fout << /*"车票种类:\n" << rhs.type << '\n' << */"车票单价:\n" << rhs.price << '\n' << "车票余量:\n" << rhs.number << '\n';
			return fout;
		}

		class tickets_overbooked : public exception {
		public:
			tickets_overbooked() : exception(
				"tickets_overbooked",
				"Your tickets have been overbooked!!!") {}
		};

		friend Attribute Ticket::modifyNumber(const Attribute& attr, const int& delta);

		friend Attribute Ticket::mergeAttribute(const Attribute& attr0, const Attribute& attr1);

		double getPrice() const {
			return price;
		}

		util::Json toJson() const {
			util::Json json;
			json["price"] = price;
			json["number"] = number;
			json["nonstop"] = nonstop;
			return json;
		}

		util::Json toJsonSimp() const {
			util::Json json;
			json["p"] = int(price * 2 + 0.5);
			json["nu"] = number;
			if (nonstop) json["no"] = int(nonstop);
			return json;
		}

		// bool operator< (const Ticket& other) {
		// 	return type < other.type;
		// }
};

Attribute modifyNumber(const Attribute& attr, const int& delta) {
	if ((int) attr.number + delta < 0)
		throw Attribute::tickets_overbooked();
	return Attribute(attr.price, attr.number + delta);
}

Attribute mergeAttribute(const Attribute& attr0, const Attribute& attr1) {
	return Attribute(attr0.price + attr1.price,
		attr0.number < attr1.number ? attr0.number : attr1.number);
}

/*
Document convertToJson(const util::pair<Type, Attribute>& ticket) {
	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();
	Value& data = initiateDocument(document, "ticket");
	{
		data.AddMember("type", Value(ticket.first.c_str(), allocator), allocator);
		data.AddMember("price", ticket.second.price, allocator);
		data.AddMember("number", (unsigned int)ticket.second.number, allocator);
		data["type"] = ticket.first;
		data["price"] = ticket.second.price;
		data["number"] = ticket.second.number;
	}

	return document;
}

util::pair<Type, Attribute> parseFromJson(const Document& document) {
	assert(std::string(document["name"].GetString()) == "ticket");
	return util::make_pair(Type(document["data"]["type"].GetString()),
		Attribute(double(document["data"]["price"].GetDouble()),
				unsigned(document["data"]["number"].GetUint64())));
}
*/

}	// Ticket

}	// TrainBoom

#endif
