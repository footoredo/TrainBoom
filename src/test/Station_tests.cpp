#include "Station.hpp"
#include <string>
using namespace TrainBoom;
using namespace std;
int main() {
	Station st(1235, "space station");
	cout <<"id: " << st.getId() << "name: " << st.getName() << endl;

	for(int i = 0; i < 100; ++i)
		st.add(1, i);
	cout << "add test ok" << endl;

	for(int i = 0; i < 100; ++i)
		st.del(1, i);
	cout << "del test ok" << endl;
	
	try {
		st.add(1, -10000);
	} catch (const Station::add_routeId_failed& e) {
		std::cout << "throw test for add ok!" << std::endl;
	}


	try {
		st.del(1, 10000);
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}


	return 0;
}

