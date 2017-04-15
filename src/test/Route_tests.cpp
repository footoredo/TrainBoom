#include "Route.hpp"
#include <string>
using namespace TrainBoom;

int main() {
    util::stupid_array<TrainBoom::id_t> stations(new TrainBoom::id_t[3]{0, 1, 2}, 3);
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
    Segment s0;
    s0.addTicket(Ticket::Type("APTV"), Ticket::Attribute(998, 10000));
    s0.addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(7773.12453, 1));
    Segment s1;
    s1.addTicket(Ticket::Type("APTV"), Ticket::Attribute(15, 15));
    s1.addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(1522, 15));
    util::stupid_array<Segment> segments(new Segment[2]{s0, s1}, 2);

    Route route(233, 3, stations, distance, arriveTime, leaveTime, segments);
    route.display();

    route.queryTickets(0, 2).display();

    TicketDelta order;
    order["APTV"] = 5;
    order["人民的名义"] = -1;

    route.modifyTickets(0, 2, order);
    route.display();
}
