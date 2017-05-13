#include "Route.hpp"
#include <string>
#include <cmath>
using namespace trainBoom;
using namespace util;
const double eps = 1e-8;

bool equal(double a, double b) {
    return std::abs(a - b) < eps;
}

int main() {
    DataManager::init();
    util::stupid_array<std::string> stations(new std::string[3]{Id("Station"), Id("Station"), Id("Station")}, 3);
    // std::cout << "sads" << std::endl;
    util::stupid_array<size_t> distance(new size_t[2]{10, 20}, 2);
    util::stupid_array<Duration> arriveTime(
        new Duration[2]{
            Duration::parse("23:39"),
            Duration::parse("24:59")
        }, 2
    );
    util::stupid_array<Duration> leaveTime(
        new Duration[2]{
            Duration::parse("23:19"),
            Duration::parse("24:49")
        }, 2
    );
    util::map<std::string, Ticket::Attribute> s0m;
    s0m.insert(util::make_pair(Ticket::Type("APTV"), Ticket::Attribute(998, 10000)));
    s0m.insert(util::make_pair(Ticket::Type("人民的名义"), Ticket::Attribute(7773.12453, 1)));
    stupid_ptr<Segment> s0 = make_stupid<Segment>(s0m);
    std::cout << "create segment done." << std::endl;
    std::cout << s0->toJson().toString() << std::endl;
    util::map<std::string, Ticket::Attribute> s1m;
    s1m.insert(util::make_pair(Ticket::Type("APTV"), Ticket::Attribute(15, 15)));
    s1m.insert(util::make_pair(Ticket::Type("人民的名义"), Ticket::Attribute(1522, 15)));
    stupid_ptr<Segment> s1 = make_stupid<Segment>(s1m);
    util::stupid_array<util::stupid_ptr<Segment>> segments(new stupid_ptr<Segment>[2]{s0, s1}, 2);

    util::stupid_array<stupid_ptr<Information>> informations(new stupid_ptr<Information>[3]{
        make_stupid<Information>(stations[0], 0, leaveTime[0], isStart),
        make_stupid<Information>(stations[1], distance[0], arriveTime[0], leaveTime[1]),
        make_stupid<Information>(stations[2], distance[1], arriveTime[1], isEnd)
    }, 3);

    // std::cout << "here" << std::endl;
    Route route("CRH", 3, informations, segments);
    // std::cout << route.toJson().toString() << std::endl;
    std::cout << "Initialization test passed!" << std::endl;
// //    route.display();
    auto q = route.queryTickets(Datetime::parse("2017/3/28"), 0, 2);
// //    q.display();
    assert(q.count("APTV") && equal(q.ticket("APTV").price, 998 + 15) && q.ticket("APTV").number == 15);
    assert(q.count("人民的名义") && equal(q.ticket("人民的名义").price, 7773.12453 + 1522) && q.ticket("人民的名义").number == 1);
    std::cout << "query test passed!" << std::endl;
//
    TicketDelta order;
    order["APTV"] = 5;
    order["人民的名义"] = -1;

    route.modifyTickets(Datetime::parse("2017/3/28"), 0, 2, order);
//    route.display();

    q = route.queryTickets(Datetime::parse("2017/3/28"), 0, 2);
// //    q.display();
    assert(q.count("APTV") && equal(q.ticket("APTV").price, 998 + 15) && q.ticket("APTV").number == 20);
    assert(q.count("人民的名义") && equal(q.ticket("人民的名义").price, 7773.12453 + 1522) && q.ticket("人民的名义").number == 0);
//
    std::cout << "modify test passed!" << std::endl;

    q = route.queryTickets(Datetime::parse("2017/3/29"), 1, 2);
    assert(q.count("APTV") && equal(q.ticket("APTV").price, 15) && q.ticket("APTV").number == 20);
    assert(q.count("人民的名义") && equal(q.ticket("人民的名义").price, 1522) && q.ticket("人民的名义").number == 14);

    try {
        route.startSelling(Datetime::parse("2017/4/1"));
        std::cout << "Duplicated start test failed!" << std::endl;
        assert(false);
    }
    catch (const Route::already_selling& e) {
        std::cout << "Duplicated start test passed!" << std::endl;
    }

    route.stopSelling(Datetime::parse("2017/4/1"));

    try {
        route.queryTickets(Datetime::parse("2017/4/1"), 0, 2);
        std::cout << "Query not selling date test failed!" << std::endl;
        assert(false);
    }
    catch (const Route::not_selling& e) {
        std::cout << "Query not selling date test passed!" << std::endl;
    }
//
//
    for (int i = 0; i < 3; ++ i) {
        // route.information(i).display();

        util::Json json = route.information(i).toJson();
        // std::cout << json.toString() << std::endl;

        // std::cout << route.information(i).getId() << std::endl;

        Information tmp(json);
        assert(tmp.toJson().toString() == json.toString());
    }
//
    util::Json routeJson = route.toJson();
    // std::cout << routeJson.toString() << std::endl;

    route.save();
    std::cout << "save done." << std::endl;
    std::string key = DataManager::finish();
    DataManager::init();
    DataManager::load(key);

    try {
        Route newRoute(Route::load(route.getId()));
        // std::cout << "done " << std::endl;
        // std::cout << newRoute.toJson().toString() << std::endl;
        assert(newRoute.toJson().toString() == route.toJson().toString());
        std::cout << "save & load test passed!" << std::endl;
    }
    catch (const exception& e) {
        std::cout << e.what() << std::endl;
        std::cout << "save & load test failed!" << std::endl;
        assert(false);
    }
    // std::cout << newRoute.toJson().toString() << std::endl;

    // std::cout << route.toString() << std::endl;
}
