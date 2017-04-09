//by ������
#ifndef TRAINBOOM_TICKET_HPP
#define TRAINBOOM_TICKET_HPP
#include <cstring>
#include <string>
#include <iostream>
using std :: ostream;

class Ticket{
	private:
		std::string type; // ��Ʊ���ࣨ����/һ��/������
		size_t price; // ��Ʊ����
		size_t number;  // ��Ʊ����

	public:
		Ticket(){ // �޲ι��죬����Ϊ��
			type ="��";
			price = number =0;
		}

		Ticket(const std::string s, size_t p = 0, size_t n = 0): price(p), number(n){
				type = s;
		}

		Ticket(const Ticket &t){
			price = t.price;
			number = t.number;
			type = t.type;
		}

		~Ticket(){}

		Ticket &operator=(const Ticket &t){
			if(&t == this)
				return *this;
			price = t.price;
			number = t.number;
			type = t.type;
			return *this;
		}

		friend ostream& operator<<(ostream &fout, const Ticket &rhs){ //�����ã���������
			fout << "��Ʊ����:\n" << rhs.type << '\n' << "��Ʊ����:\n" << rhs.price << '\n' << "��Ʊ����:\n" << rhs.number << '\n';
			return fout;
		}

	};

#endif
