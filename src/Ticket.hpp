//by 巫宇威
#ifndef TRAINBOOM_TICKET_HPP
#define TRAINBOOM_TICKET_HPP
#include <cstring>
#include <string>
#include <iostream>
using std :: ostream;

class Ticket{
	private:
		std::string type; // 车票种类（二等/一等/商务）
		double price; // 车票单价
		// size_t number;  // 车票余量

	public:
		Ticket(): type(), price() { // 无参构造，类型为空
		}

		Ticket(const std::string s, double p): type(s), price(p) {}

		Ticket(const Ticket &t){
			price = t.price;
			// number = t.number;
			type = t.type;
		}

		~Ticket(){}

		Ticket &operator=(const Ticket &t){
			if(&t == this)
				return *this;
			price = t.price;
			// number = t.number;
			type = t.type;
			return *this;
		}

		friend ostream& operator<<(ostream &fout, const Ticket &rhs){ //测试用，重载输出
			fout << "车票种类:\n" << rhs.type << '\n' << "车票单价:\n" << rhs.price << '\n' /*<< "车票余量:\n" << rhs.number << '\n'*/;
			return fout;
		}

		bool operator< (const Ticket& other) {
			return type < other.type;
		}
	};

#endif
