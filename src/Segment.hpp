#ifndef TRAINBOOM_SEGMENT_HPP
#define TRAINBOOM_SEGMENT_HPP

#include <iostream>
#include "util.hpp"
#include "exception.hpp"
#include "Ticket.hpp"

class Segment {
private:
    util::map<Ticket::Type, Ticket::Attribute> tickets;

public:
    class ticket_not_found : public exception {
    public:
        ticket_not_found() : exception(
            "ticket_not_found",
            "Your requiring ticket is not fonud!!!") {}
    };

    class tickets_set_not_match : public exception {
    public:
        tickets_set_not_match() : exception(
            "tickets_set_not_match",
            "Your tickets sets do not match!!!") {}
    };

    class ticket_already_exists : public exception {
    public:
        ticket_already_exists() : exception(
            "ticket_already_exists",
            "The ticket you insert already exists!!!") {}
    };

    class Modifier {
        friend class Segment;
    public:
        Segment operator()(const Segment& segment, const util::pair<Ticket::Type, int>& delta) {
            Segment ret(segment);
            auto iter(ret.tickets.find(delta.first));
            if (iter == ret.tickets.end())
                throw ticket_not_found();
            else {
                iter->second = Ticket::modifyNumber(iter->second, delta.second);
            }
            return ret;
        }
    };

    class MergerT {
        friend class Segment;
    public:
        Segment operator()(const Segment& segment0, const Segment& segment1) {
            if (segment0.tickets.size() != segment1.tickets.size())
                throw tickets_set_not_match();
            Segment ret(segment0);
            for (auto& item0 : ret.tickets) {
                const auto& iter1(segment1.tickets.find(item0.first));
                if (iter1 == segment1.tickets.end())
                    throw tickets_set_not_match();
                item0.second = Ticket::mergeAttribute(item0.second, iter1->second);
            }
            return ret;
        }
    };

    constexpr Segment() noexcept: tickets() {}
    Segment(const util::map<Ticket::Type, Ticket::Attribute>& tickets) noexcept: tickets(tickets) {}
    Segment(const Segment& other) noexcept: tickets(other.tickets) {}

    void addTicket(const Ticket::Type& type, const Ticket::Attribute& attr) {
        auto retValue = tickets.insert(util::make_pair(type, attr));
        if (!retValue.second)
            throw ticket_already_exists();
    }

    void display() const {
        for (const auto& item : tickets) {
            std::cout << item.first << "\n" << item.second << std::endl;
        }
    }
};

#endif
