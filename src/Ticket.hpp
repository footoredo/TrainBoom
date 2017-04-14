//by 巫宇威
#ifndef TRAINBOOM_TICKET_HPP
#define TRAINBOOM_TICKET_HPP
#include <cstring>
#include <string>
#include <iostream>
#include "exception.hpp"
using std :: ostream;

namespace Ticket {

typedef std::string Type;

class Attribute {
	private:
		// std::string type; // 车票种类（二等/一等/商务）
		double price; // 车票单价
		size_t number;  // 车票余量

	public:
		Attribute(): price(), number() { // 无参构造，类型为空
		}

		Attribute(double p, size_t n): price(p), number(n) {}

		Attribute(const Attribute &t): price(t.price), number(t.number) {
			// price = t.price;
			// number = t.number;
			// type = t.type;
		}

		~Attribute(){}

		Attribute &operator=(const Attribute &t){
			if(&t == this)
				return *this;
			price = t.price;
			number = t.number;
			// type = t.type;
			return *this;
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

}

#endif
