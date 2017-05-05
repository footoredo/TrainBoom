#include "Route.hpp"
#include <string>
using namespace TrainBoom;
using namespace util;
const double eps = 1e-8;

int main() {
    util::stupid_array<TrainBoom::Id> stations(new TrainBoom::Id[3]{Id("Station"), Id("Station"), Id("Station")}, 3);
    // std::cout << "sads" << std::endl;
    util::stupid_array<size_t> distance(new size_t[2]{10, 20}, 2);
    util::stupid_array<util::Datetime::Datetime> arriveTime(
        new util::Datetime::Datetime[2]{
            util::Datetime::Datetime::parse("2017/4/14 23:39"),
            util::Datetime::Datetime::parse("2017/4/14 23:59")
        }, 2
    );
    util::stupid_array<util::Datetime::Datetime> leaveTime(
        new util::Datetime::Datetime[2]{
            util::Datetime::Datetime::parse("2017/4/14 23:19"),
            util::Datetime::Datetime::parse("2017/4/14 23:49")
        }, 2
    );
    stupid_ptr<Segment> s0 = make_stupid<Segment>();
    s0->addTicket(Ticket::Type("APTV"), Ticket::Attribute(998, 10000));
    s0->addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(7773.12453, 1));
    stupid_ptr<Segment> s1 = make_stupid<Segment>();
    s1->addTicket(Ticket::Type("APTV"), Ticket::Attribute(15, 15));
    s1->addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(1522, 15));
    util::stupid_array<util::stupid_ptr<Segment>> segments(new stupid_ptr<Segment>[2]{s0, s1}, 2);

    util::stupid_array<stupid_ptr<Information>> informations(new stupid_ptr<Information>[3]{
        make_stupid<Information>(stations[0], 0, leaveTime[0], isStart),
        make_stupid<Information>(stations[1], distance[0], arriveTime[0], leaveTime[1]),
        make_stupid<Information>(stations[2], distance[1], arriveTime[1], isEnd)
    }, 3);

    Route route("CRH", 3, informations, segments);
    std::cout << "Initialization test passed!" << std::endl;
//    route.display();
    auto q = route.queryTickets(stations[0], stations[2]);
//    q.display();
    assert(q.getTickets().count("APTV") && equal(q.getTickets()["APTV"].price, 998 + 15) && q.getTickets()["APTV"].number == 15);
    assert(q.getTickets().count("人民的名义") && equal(q.getTickets()["人民的名义"].price, 7773.12453 + 1522) && q.getTickets()["人民的名义"].number == 1);
    std::cout << "query test passed!" << std::endl;

    TicketDelta order;
    order["APTV"] = 5;
    order["人民的名义"] = -1;

    route.modifyTickets(stations[0], stations[2], order);
//    route.display();


    q = route.queryTickets(stations[0], stations[2]);
//    q.display();
    assert(q.getTickets().count("APTV") && equal(q.getTickets()["APTV"].price, 998 + 15) && q.getTickets()["APTV"].number == 20);
    assert(q.getTickets().count("人民的名义") && equal(q.getTickets()["人民的名义"].price, 7773.12453 + 1522) && q.getTickets()["人民的名义"].number == 0);

    std::cout << "modify test passed!" << std::endl;


    for (int i = 0; i < 3; ++ i) {
        // route.information(i).display();

        util::Json json = route.information(i).toJson();
        std::cout << json.toString() << std::endl;

        std::cout << route.information(i).getId() << std::endl;

        Information tmp(json);
        tmp.display();
    }

    util::Json routeJson = route.toJson();
    std::cout << routeJson.toString() << std::endl;

    std::cout << route.toString() << std::endl;
}
