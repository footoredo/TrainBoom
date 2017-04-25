#include <iostream>
#include "Segment.hpp"
#include "util.hpp"
using namespace TrainBoom;

int main() {
    Segment s0;
    s0.addTicket(Ticket::Type("APTV"), Ticket::Attribute(998, 10000));
    s0.addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(7773.12453, 1));
    s0.display();

    Segment s1;
    s1.addTicket(Ticket::Type("APTV"), Ticket::Attribute(15, 15));

    try {
        Segment::MergerT()(s0, s1).display();
    } catch (const Segment::tickets_set_not_match& e) {
        std::cout << "throw ok!\n";
    }

    try {
        s1.addTicket(Ticket::Type("APTV"), Ticket::Attribute(15, 15));
    } catch (const Segment::ticket_already_exists& e) {
        std::cout << "throw ok!\n";
    }

    s1.addTicket(Ticket::Type("人民的名义"), Ticket::Attribute(1522, 15));
    s1.display();

    Segment::MergerT()(s0, s1).display();

    TicketDelta ticketDelta;
    ticketDelta["人民的名义"] = -6;
    Segment::Modifier()(s1, ticketDelta, 0, 0).display();

    util::Json json = s1.toJson();
    std::cout << json.toString() << std::endl;

    Segment s2(json);
    s2.display();
}
