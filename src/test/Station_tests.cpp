#include "Station.hpp"
#include "util/Datetime.hpp"
#include <string>
using namespace TrainBoom;
using namespace std;
using util::Datetime::Datetime;

std::string stationId[50], routeId[100];

int main() {
    DataManager::init();
    Datetime today = Datetime::parse("2017/5/5");
	Station st("space station");
	cout <<"id: " << st.getId() << std::endl << "name: " << st.getName() << endl;

	for(int i = 0; i < 100; ++i) {
		if (i < 50) stationId[i] = Id("Station");
		routeId[i] = Id("Route");
 //       if (i < 10) std::cout << i << ": " << routeId[i] << std::endl;
		st.add(stationId[i / 2], today, RouteInterval(routeId[i], 0, 1));
	}
	cout << "add test ok" << endl;

	for(int i = 50; i < 100; ++i) {
		st.del(stationId[i / 2], today, RouteInterval(routeId[i], 0, 1));
	}
	cout << "del test ok" << endl;

	/*try {
		st.add(1, -10000);
	} catch (const Station::add_routeId_failed& e) {
		std::cout << "throw test for add ok!" << std::endl;
	}*/


	try {
		st.del(stationId[0], today, RouteInterval(Id("Route"), 0, 1));
        std::cout << "throw test for del failed!" << std::endl;
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}

    for (int i = 0; i < 25; ++ i) {
        // std::cout << i << std::endl;
        const auto routes = st.query(stationId[i], today);
        assert(routes.size() == 2);
//        std::cout << routes[0] << " " << routes[1] << std::endl;
        assert((routes[0].routeId == routeId[i * 2] && routes[1].routeId == routeId[i * 2 + 1]) || (routes[1].routeId == routeId[i * 2] && routes[0].routeId == routeId[i * 2 + 1]));
    }
    std::cout << "query test ok!" << std::endl;

    st.save();
    std::cout << "done save." << std::endl;
    Station new_st(st.getId(), DataManager::getFile(st.getId()));
    // std::cout << new_st.toJson().toString() << std::endl;
    assert(new_st.toJson().toString() == st.toJson().toString());
    std::cout << "save & load test passed!" << std::endl;

    try {
		new_st.del(stationId[0], today, RouteInterval(Id("Route"), 0, 1));
        std::cout << "throw test for del failed!" << std::endl;
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}

    for (int i = 0; i < 25; ++ i) {
        // std::cout << i << std::endl;
        const auto routes = new_st.query(stationId[i], today);
        assert(routes.size() == 2);
//        std::cout << routes[0] << " " << routes[1] << std::endl;
        assert((routes[0].routeId == routeId[i * 2] && routes[1].routeId == routeId[i * 2 + 1]) || (routes[1].routeId == routeId[i * 2] && routes[0].routeId == routeId[i * 2 + 1]));
    }
    std::cout << "query test ok!" << std::endl;

    // std::cout << st.queryJson(stationId[0], today).toString() << std::endl;

	return 0;
}
