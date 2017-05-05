#include<iostream>
#include "Ticket.hpp"
#include "util.hpp"
using namespace std;
using namespace TrainBoom;
using namespace Ticket;

int main(){
	Ticket::Attribute t1(1000.5, 12);
    Ticket::Attribute t2(t1);
	Ticket::Attribute t3;
	Ticket::Attribute t4 = t1;
	cout << "t3:\n" << t3 << endl;
	t3 = t1;
	cout <<"t1:\n" << t1 << endl << "t2:\n" << t2 << endl << "t3:\n" << t3 << endl << "t4:\n" << t4 << endl;

	Ticket::Attribute t5(20, 100);
	std::cout << "t5:\n" << t5 << std::endl;
	cout << "merge: " << mergeAttribute(t1, t5) << std::endl;

	cout << "modify: " << modifyNumber(t5, -20) << std::endl;

	try {
		modifyNumber(t5, -120);
	} catch (const Attribute::tickets_overbooked& e) {
		std::cout << "throw test ok!" << std::endl;
	}

	// auto ticket = util::make_pair(Ticket::Type("火箭舱"), t1);
	util::Json json = t1.toJson();
	std::cout << json.toString() << std::endl;

	Ticket::Attribute t6(json);
	std::cout << t6 << std::endl;
    
    std::cout << Ticket::Attribute(Json("attribute").Parse("{\"price\":123.5}")) << std::endl;

	return 0;
}
