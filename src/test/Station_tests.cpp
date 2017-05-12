#include "Station.hpp"
#include "util/Datetime.hpp"
#include "DataManager2.hpp"
#include <string>
using namespace trainBoom;
using namespace std;
using util::date_time::Datetime;

std::string stationName[50], routeId[100], routeName[100];

int main() {
    DataManager::init();
    // Datetime today = Datetime::parse("2017/5/5");
	Station st("space-station");
	cout <<"id: " << st.getId() << std::endl << "name: " << st.getName() << endl;

	for(int i = 0; i < 100; ++i) {
		if (i < 50) stationName[i] = "station-" + getSalt(4);
		routeId[i] = Id("Route");
        routeName[i] = "route-" + getSalt(4);
 //       if (i < 10) std::cout << i << ": " << routeId[i] << std::endl;
        // if (i >= 50 && i < 52) std::cout << stationName[i / 2] << " " << routeName[i] << std::endl;
		st.add(stationName[i / 2], RouteInterval(routeId[i], routeName[i], 0, 1));
	}
	cout << "add test ok" << endl;

	for(int i = 50; i < 100; ++i) {
        //    std::cout << stationName[i / 2] << " " << routeName[i] << std::endl;
		st.del(stationName[i / 2], RouteInterval(routeId[i], routeName[i], 0, 1));
	}
	cout << "del test ok" << endl;

	/*try {
		st.add(1, -10000);
	} catch (const Station::add_routeId_failed& e) {
		std::cout << "throw test for add ok!" << std::endl;
	}*/


	try {
		st.del(stationName[0], RouteInterval(Id("Route"), getSalt(4), 0, 1));
        std::cout << "throw test for del failed!" << std::endl;
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}

    for (int i = 0; i < 25; ++ i) {
        // std::cout << i << std::endl;
        const auto routes = st.query(stationName[i]);
        assert(routes.size() == 2);
//        std::cout << routes[0] << " " << routes[1] << std::endl;
        assert((routes[0].routeId == routeId[i * 2] && routes[1].routeId == routeId[i * 2 + 1]) || (routes[1].routeId == routeId[i * 2] && routes[0].routeId == routeId[i * 2 + 1]));
    }
    const auto route = st.query(stationName[0], routeName[0]);
    assert(route.routeId == routeId[0]);
    std::cout << "query test ok!" << std::endl;

    st.save();
    std::cout << "done save." << std::endl;
    std::string key = DataManager::finish();
    DataManager::init();
    DataManager::load(key);

    Station new_st(Station::load(st.getId()));
    // std::cout << st.toJson().toString() << std::endl;
    // std::cout << new_st.toJson().toString() << std::endl;
    assert(new_st.toJson().toString() == st.toJson().toString());
    std::cout << "save & load test passed!" << std::endl;

    try {
		new_st.del(stationName[0], RouteInterval(Id("Route"), getSalt(4), 0, 1));
        std::cout << "throw test for del failed!" << std::endl;
	} catch (const Station::delete_routeId_failed& e) {
		std::cout << "throw test for del ok!" << std::endl;
	}

    for (int i = 0; i < 25; ++ i) {
        // std::cout << i << std::endl;
        const auto routes = new_st.query(stationName[i]);
        assert(routes.size() == 2);
//        std::cout << routes[0] << " " << routes[1] << std::endl;
        assert((routes[0].routeId == routeId[i * 2] && routes[1].routeId == routeId[i * 2 + 1]) || (routes[1].routeId == routeId[i * 2] && routes[0].routeId == routeId[i * 2 + 1]));
    }
    const auto route2 = new_st.query(stationName[0], routeName[0]);
    assert(route2.routeId == routeId[0]);
    std::cout << "query test ok!" << std::endl;

    // std::cout << st.queryJson(stationName[0], today).toString() << std::endl;

	return 0;
}
