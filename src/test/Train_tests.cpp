#include "Train.hpp"
#include <iostream>

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

    util::stupid_array<Information> informations(new Information[3]{
        Information(stations[0], 0, leaveTime[0], isStart),
        Information(stations[1], distance[0], arriveTime[0], leaveTime[1]),
        Information(stations[2], distance[1], arriveTime[1], isEnd)
    }, 3);

    Route route(233, 3, informations, segments);

    Train train(0, "CRH 233", route);

    train.route.display();
    std::cout << train.getRunningDay() << std::endl;
}
