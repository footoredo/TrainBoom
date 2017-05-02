#include "Station.hpp"
#include <string>
using namespace TrainBoom;
using namespace std;

Id* stationId[100], *routeId[100];

int main() {
	Station st("space station");
	cout <<"id: " << st.getId() << "name: " << st.getName() << endl;

	for(int i = 0; i < 100; ++i) {
		stationId[i] = new Id("Station");
		routeId[i] = new Id("Route");
		st.add(*stationId[i], *routeId[i]);
	}
	cout << "add test ok" << endl;

	for(int i = 0; i < 100; ++i) {
		st.del(*stationId[i], *routeId[i]);
		delete stationId[i];
		delete routeId[i];
	}
	cout << "del test ok" << endl;

	/*try {
		st.add(1, -10000);
	} catch (const Station::add_routeId_failed& e) {
		std::cout << "throw test for add ok!" << std::endl;
	}


	try {
		st.del();
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}
	*/

	return 0;
}
