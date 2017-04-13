#include<iostream>
#include"Ticket.hpp"
using namespace std;

int main(){
	Ticket t1("一等座", 1000.5);
    Ticket t2(t1);
	Ticket t3;
	Ticket t4 = t1;
	cout << "t3:\n" << t3 << endl;
	t3 = t1;
	cout <<"t1:\n" << t1 << endl << "t2:\n" << t2 << endl << "t3:\n" << t3 << endl << "t4:\n" << t4 << endl;

	return 0;
}
