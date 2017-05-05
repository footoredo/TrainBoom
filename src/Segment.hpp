#ifndef TRAINBOOM_SEGMENT_HPP
#define TRAINBOOM_SEGMENT_HPP

#include <iostream>
#include "util.hpp"
#include "exception.hpp"
#include "Ticket.hpp"

namespace TrainBoom {

typedef util::map<Ticket::Type, int> TicketDelta;

class Segment {
private:
    util::map<Ticket::Type, Ticket::Attribute> tickets;
    Id id;

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
        Segment operator()(const Segment& segment, const TicketDelta& deltas, size_t l, size_t r) {
            Segment ret(segment);
            for (const auto& delta : deltas) {
                auto iter(ret.tickets.find(delta.first));
                if (iter == ret.tickets.end())
                    throw ticket_not_found();
                else {
                    iter->second = Ticket::modifyNumber(iter->second, delta.second);
                }
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

    class MergerM {
    public:
        TicketDelta operator()(const TicketDelta& deltas0, const TicketDelta& deltas1) {
            TicketDelta ret(deltas0);
            for (const auto& delta : deltas1) {
                ret[delta.first] += delta.second;
            }
            return ret;
        }
    };

    Segment() noexcept: tickets(), id("Segment") {}
    Segment(const util::map<Ticket::Type, Ticket::Attribute>& tickets) noexcept
        : tickets(tickets), id("Segment") {}
    Segment(const Segment& other) noexcept: tickets(other.tickets), id("Segment") {}
    Segment(const util::Json& json): id("Segment") {
//        assert(json.getType() == "segment");
        json["tickets"].forEach([this](const std::string& type, util::Json attribute) {
            tickets[type] = Ticket::Attribute(attribute);
        });
    }

    const util::map<Ticket::Type, Ticket::Attribute>& getTickets() const {
        return tickets;
    }

    Ticket::Attribute ticket(const Ticket::Type& ticketType) const {
        if (!tickets.count(ticketType))
            throw ticket_not_found();
        return tickets.at(ticketType);
    }

    /*Segment(const Value& jsonValue) {
        assert(jsonValue.IsObject());
        assert(std::string(jsonValue["name"].GetString()) == "segment");
        for (const auto& ticketObject : jsonValue["data"].GetObject()) {
            tickets[std::string(ticketObject.name.GetString())] =
                Ticket::Attribute(ticketObject.value);
        }
    }*/

    void addTicket(const Ticket::Type& type, const Ticket::Attribute& attr) {
        auto retValue = tickets.insert(util::make_pair(type, attr));
        if (!retValue.second)
            throw ticket_already_exists();
    }

    void display() const {
        std::cout << "#####################" << std::endl;
        for (const auto& item : tickets) {
            std::cout << item.first << "\n" << item.second << std::endl;
        }
        std::cout << "#####################" << std::endl;
    }

    util::Json toJson() const {
        util::Json json("segment", id);
        json["tickets"].SetObject();

        for (const auto& ticket: tickets) {
            json["tickets"][ticket.first] = ticket.second.toJson();
        }

        return json;
    }

    std::string toString() const {
        std::stringstream ss;
        // ss << "createTime " << createTime << '\n';
        ss << "tickets " << tickets.size() << '\n';
        for (const auto& ticket: tickets)
            ss << "ticket " << ticket.first << " " << ticket.second.price << " " << ticket.second.number << '\n';
        return ss.str();
    }

    Id getId() const {
        return id;
    }
};

}   // TrainBoom

#endif
