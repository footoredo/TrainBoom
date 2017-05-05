#include "Station.hpp"
#include <string>
using namespace TrainBoom;
using namespace std;
using util::Datetime::Datetime;

std::string stationId[50], routeId[100];

int main() {
    Datetime today = Datetime::parse("2017/5/5");
	Station st("space station");
	cout <<"id: " << st.getId() << std::endl << "name: " << st.getName() << endl;

	for(int i = 0; i < 100; ++i) {
		if (i < 50) stationId[i] = Id("Station");
		routeId[i] = Id("Route");
 //       if (i < 10) std::cout << i << ": " << routeId[i] << std::endl;
		st.add(stationId[i / 2], today, routeId[i]);
	}
	cout << "add test ok" << endl;

	for(int i = 50; i < 100; ++i) {
		st.del(stationId[i / 2], today, routeId[i]);
	}
	cout << "del test ok" << endl;

	/*try {
		st.add(1, -10000);
	} catch (const Station::add_routeId_failed& e) {
		std::cout << "throw test for add ok!" << std::endl;
	}*/


	try {
		st.del(stationId[0], today, Id("Route"));
        std::cout << "throw test for del failed!" << std::endl;
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}

    for (int i = 0; i < 25; ++ i) {
        const auto routes = st.query(stationId[i], today);
        assert(routes.size() == 2);
//        std::cout << routes[0] << " " << routes[1] << std::endl;
        assert((routes[0] == routeId[i * 2] && routes[1] == routeId[i * 2 + 1]) || (routes[1] == routeId[i * 2] && routes[0] == routeId[i * 2 + 1]));
    }
    std::cout << "query test ok!" << std::endl;

    std::cout << st.queryJson(stationId[0], today).toString() << std::endl;

	return 0;
}
