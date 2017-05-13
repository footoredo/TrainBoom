#ifndef TRAINBOOM_SEGMENT_HPP
#define TRAINBOOM_SEGMENT_HPP

#include <iostream>
#include "util/map.hpp"
#include "exception.hpp"
#include "Ticket.hpp"

namespace trainBoom {

typedef util::map<std::string, int> TicketDelta;

class Segment {
private:
    // util::map<int, Ticket::Attribute> tickets;
    // std::string id;
    size_t _size;
    stupid_array<util::pair<std::string, Ticket::Attribute>> tickets;

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

/*
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
    };*/

    Segment() noexcept {}
    Segment(const util::map<std::string, Ticket::Attribute>& _tickets) {
        _size = _tickets.size();
        // type = make_array<std::string>(size);
        // attr = make_array<Ticket::Attribute>(size);
        tickets = make_array<util::pair<std::string, Ticket::Attribute>>(_size);
        size_t i = 0;
        for (const auto& item: _tickets) {
            tickets[i].first = item.first;
            tickets[i].second = item.second;
            ++ i;
        }
        tickets.sort();
    }
    Segment(const Segment& other) {
        _size = other._size;
        // type = make_array<std::string>(_size);
        // attr = make_array<Ticket::Attribute>(_size);
        tickets = make_array<util::pair<std::string, Ticket::Attribute>>(_size);
        for (size_t i = 0; i < _size; ++ i) {
            // type[i] = other.type[i];
            // attr[i] = other.attr[i];
            tickets[i] = other.tickets[i];
        }
    }
    Segment(const util::Json& json) {
//        assert(json.getType() == "segment");
        // if (json.getId() != "") id = json.getId();
        // else id = Id("Segment");
        _size = json["tickets"].Size();
        // type = make_array<std::string>(_size);
        // attr = make_array<Ticket::Attribute>(_size);
        tickets = make_array<util::pair<std::string, Ticket::Attribute>>(_size);
        size_t i = 0;
        json["tickets"].forEach([&](const std::string& _type, util::Json _attr) {
            // type[i] = _type;
            // attr[i] = Ticket::Attribute(_attr);
            tickets[i] = util::make_pair(_type, Ticket::Attribute(_attr));
            ++ i;
        });
        tickets.sort();
    }

    stupid_array<util::pair<std::string, Ticket::Attribute>> getTickets() {
        return tickets;
    }

    Segment& operator=(const Segment& other) {
        if (this != &other) {
            _size = other._size;
            tickets = make_array<util::pair<std::string, Ticket::Attribute>>(_size);
            for (size_t i = 0; i < _size; ++ i) {
                tickets[i] = other.tickets[i];
            }
        }
        return *this;
    }

    size_t count(std::string ticketType) const {
        size_t ans = 0;
        for (size_t i = 0; i < _size; ++ i)
            if (tickets[i].first == ticketType)
                ++ ans;
        return ans;
    }

/*
    const util::map<int, Ticket::Attribute>& getTickets() const {
        return tickets;
    }*/

    size_t find(std::string ticketType) const {
        for (size_t i = 0; i < _size; ++ i)
            if (tickets[i].first == ticketType)
                return i;
        throw ticket_not_found();
    }

    Ticket::Attribute& ticket(const Ticket::Type& ticketType) {
        return tickets[find(ticketType)].second;
    }

    const Ticket::Attribute& ticket(const Ticket::Type& ticketType) const {
        return tickets[find(ticketType)].second;
    }

    size_t size() const {
        return _size;
    }

    Segment operator+(const Segment& other) const {
        Segment ret;
        assert(_size == other._size);
        ret._size = _size;
        ret.tickets = make_array<util::pair<std::string, Ticket::Attribute>>(_size);
        for (size_t i = 0; i < _size; ++ i) {
            assert(tickets[i].first == other.tickets[i].first);
            ret.tickets[i] = util::make_pair(tickets[i].first, mergeAttribute(tickets[i].second, other.tickets[i].second));
        }
        return ret;
    }

    /*Segment(const Value& jsonValue) {
        assert(jsonValue.IsObject());
        assert(std::string(jsonValue["name"].GetString()) == "segment");
        for (const auto& ticketObject : jsonValue["data"].GetObject()) {
            tickets[std::string(ticketObject.name.GetString())] =
                Ticket::Attribute(ticketObject.value);
        }
    }*/
/*
    void addTicket(const Ticket::Type& type, const Ticket::Attribute& attr) {
        auto retValue = tickets.insert(util::make_pair(rand(), attr));
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
*/
    util::Json toJson() const {
        util::Json json;
        json["tickets"].SetObject();

        for (size_t i = 0; i < _size; ++ i) {
            json["tickets"][tickets[i].first] = tickets[i].second.toJson();
        }

        return json;
    }
/*
    std::string toString() const {
        std::stringstream ss;
        // ss << "createTime " << createTime << '\n';
        ss << "tickets " << tickets.size() << '\n';
        for (const auto& ticket: tickets)
            ss << "ticket " << ticket.first << " " << ticket.second.price << " " << ticket.second.number << '\n';
        return ss.str();
    }*/
};

}   // TrainBoom

#endif
