#include "TrainBoom.hpp"
#include "DataManager2.hpp"
#include "util/log.hpp"
#include <string>

#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"

namespace trainBoom {

    namespace API {

        using namespace Net;

        class StatsEndpoint {
            private:
                void sendJson(Net::Http::ResponseWriter& response, const Json& json) {
                    using namespace Net::Http;
                    response.headers()
                        .add<Header::ContentType>(MIME(Application, Json));

                    auto stream = response.stream(Net::Http::Code::Ok);
                    stream << json.toString().c_str() << ends;
                    api_log->log("response => " + json.toString());
                }

                Json error(const std::string& errMsg) {
                    Json err("error"); err["errMsg"] = errMsg;
                    return err;
                }

                Json success(const std::string& succMsg) {
                    Json succ("success"); succ["succMsg"] = succMsg;
                    return succ;
                }

                Json vec2Json(const util::vector<std::string>& vec, std::string type) {
                    Json json;
                    json[type + "s"].SetArray();
                    for (const std::string& item: vec)
                        json[type + "s"].PushBack(item);
                    return json;
                }

                template <class T>
                    Json vec2Json(const util::vector<T>& vec, std::string type) {
                        Json json;
                        json[type + "s"].SetArray();
                        for (const T& item: vec)
                            json[type + "s"].PushBack(item.toJson());
                        return json;
                    }
#define SENDJSON(json) sendJson(response, json)
#define SENDOBJ(obj) SENDJSON(obj.toJson())
#define SENDVEC(vec, type) SENDJSON(vec2Json(vec, type))
#define SENDSUCC(succMsg) sendJson(response, success(succMsg))
#define SENDERR(errMsg) sendJson(response, error(errMsg))
#define APIHANDLER(APIName) void APIName(const Rest::Request& request, Net::Http::ResponseWriter response)
#define ROUTING(method, path, func) Routes::method(router, path, Routes::bind(&StatsEndpoint::func, this))
#define HANDLEERR catch (const exception& e) { SENDERR(e.what()); }

            public:
                bool shutdownFlag;
                StatsEndpoint(Net::Address addr, const util::stupid_ptr<TrainBoom>& trainBoom,
                    util::stupid_ptr<Log> user_log,
                    util::stupid_ptr<Log> route_log,
                    util::stupid_ptr<Log> station_log,
                    util::stupid_ptr<Log> api_log)
                    : httpEndpoint(util::make_stupid<Net::Http::Endpoint>(addr)), trainBoom(trainBoom),
                      user_log(user_log),
                      route_log(route_log),
                      station_log(station_log),
                      api_log(api_log) {
                      }

                void init(size_t thr = 2) {
                    auto opts = Net::Http::Endpoint::options()
                        .threads(thr)
                        .flags(Net::Tcp::Options::InstallSignalHandler);
                    httpEndpoint->init(opts);
                    setupRoutes();
                }

                void start() {
                    httpEndpoint->setHandler(router.handler());
                    httpEndpoint->serveThreaded();
                }

                void shutdown() {
                    httpEndpoint->shutdown();
                }

            private:
                void setupRoutes() {
                    using namespace Net::Rest;

                    ROUTING(Get, "/save", saveTrainBoom);
                    Routes::Get(router, "/shutdown", Routes::bind(&StatsEndpoint::_shutdown, this));

                    Routes::Post(router, "/users", Routes::bind(&StatsEndpoint::insertUser, this));
                    Routes::Get(router, "/users", Routes::bind(&StatsEndpoint::listUsers, this));
                    Routes::Get(router, "/users/:userId", Routes::bind(&StatsEndpoint::getUser, this));
                    Routes::Put(router, "/users/:userId", Routes::bind(&StatsEndpoint::updateUser, this));
                    ROUTING(Delete, "/users/:userId", deleteUser);
                    ROUTING(Get, "/users/:userId/orders", listOrdersUser);
                    ROUTING(Get, "/users/:userId/orders/:orderId", getOrderUser);
                    ROUTING(Post, "/users/username", getByUsername);
                    //      Routes::Get(router, "/users/:userId/orders", Routes::bind(&StatsEndpoint::listOrdersUser, this));
                    //    Routes::Get(router, "/users/:userId/orders/:orderId", Routes::bind(&StatsEndpoint::getOrderUser, this));

                    Routes::Post(router, "/stations", Routes::bind(&StatsEndpoint::insertStation, this));
                    Routes::Get(router, "/stations", Routes::bind(&StatsEndpoint::listStations, this));
                    Routes::Get(router, "/stations/:stationId", Routes::bind(&StatsEndpoint::getStation, this));
                    Routes::Put(router, "/stations/:stationId", Routes::bind(&StatsEndpoint::updateStation, this));
                    ROUTING(Delete, "/stations/:stationId", deleteStation);
                    ROUTING(Post, "/stations/name", getByStationName);
                    // Routes::Post(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::addRouteStation, this));
                    // Routes::Delete(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::delRouteStation, this));
                    Routes::Post(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::queryRouteStation, this));

                    Routes::Post(router, "/routes", Routes::bind(&StatsEndpoint::insertRoute, this));
                    Routes::Get(router, "/routes", Routes::bind(&StatsEndpoint::listRoutes, this));
                    Routes::Get(router, "/routes/:routeId", Routes::bind(&StatsEndpoint::getRoute, this));
                    ROUTING(Put, "/routes/:routeId", updateRoute);
                    ROUTING(Delete, "/routes/:routeId", deleteRoute);
                    ROUTING(Post, "routes/name", getByRouteName);
                    //                    Routes::Put(router,)
                    //Routes::Get(router, "/routes/:routeId/start", Routes::bind(&StatsEndpoint::startRoute, this));
                    //Routes::Get(router, "/routes/:routeId/stop", Routes::bind(&StatsEndpoint::stopRoute, this));
                    Routes::Post(router, "/routes/:routeId/tickets", Routes::bind(&StatsEndpoint::queryTicketsRoute, this));
                    Routes::Post(router, "/routes/:routeId/tickets/book", Routes::bind(&StatsEndpoint::bookTicketsRoute, this));
                    Routes::Post(router, "/routes/:routeId/tickets/refund", Routes::bind(&StatsEndpoint::refundTicketsRoute, this));

                    ROUTING(Post, "/routeInterval/get", getRouteInterval);
                    //                    ROUTING(Post, "/routeInterval/query", queryRouteInterval);
                    //                  ROUTING(Post, "/routeInterval/book", bookRouteInterval);
                }

                APIHANDLER(saveTrainBoom) {
                    response.send(Http::Code::Ok, "[" + trainBoom->getId() + "] saving...");
                    trainBoom->save();
                    std::string key = DataManager::finish();
                    api_log->log("save key [" + key + "] + TrainBoom id [" + trainBoom->getId() + "]");
                    std::cout << "key: " << key << std::endl;
                    std::cout << "TrainBoom id: " << trainBoom->getId() << std::endl;
                    std::cout << "save done." << std::endl;
                }

                void _shutdown(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::cout << "Received shutdown." << std::endl;
                    response.send(Http::Code::Ok, "Shutdown.");
                    //                    shutdown();
                    shutdownFlag = true;
                }

                void insertUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    try {
                        Json tmp; tmp.Parse(request.body());
                        api_log->log("insert user");
                        api_log->log("request => " + tmp.toString());
                        User user(tmp);
                        trainBoom->insertUser(user);
                        user_log->log("insert [" + user.getId() + ":" + user.getUsername() + "]");

                        sendJson(response, user.toJson());
                    }
                    HANDLEERR;
                }

                void listUsers(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    api_log->log("list users");
                    sendJson(response, vec2Json(trainBoom->listUsers(), "user"));
                }

                void getUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
                    api_log->log("get user [" + userId + "]");
                    //                    std::cout << userId << std::endl;
                    try {
                        const User& user = trainBoom->user(userId);
                        sendJson(response, user.toJson());
                    }
                    HANDLEERR;
                }

                void updateUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
                    api_log->log("update user [" + userId + "] ");
                    Json tmp; tmp.Parse(request.body());
                    api_log->log("request => " + tmp.toString());
                    //                    std::cout << userId << std::endl;
                    try {
                        User& user = trainBoom->user(userId);
                        try {
                            user.update(tmp);
                            user_log->log("update [" + user.getId() + ":" + user.getUsername() + "]");
                            sendJson(response, user.toJson());
                        }
                        catch (const User::information_missing& e) {
                            sendJson(response, error(e.what()));
                        }
                    }
                    HANDLEERR;
                }

                APIHANDLER(deleteUser) {
                    std::string userId = request.param(":userId").as<std::string>();
                    api_log->log("delete user [" + userId + "] ");
                    try {
                        User& user = trainBoom->user(userId);
                        trainBoom->deleteUser(userId);
                        user_log->log("delete [" + user.getId() + ":" + user.getUsername() + "]");
                        SENDSUCC("Delete user succeeded!");
                    }
                    HANDLEERR;
                }

                APIHANDLER(listOrdersUser) {
                    std::string userId = request.param(":userId").as<std::string>();
                    api_log->log("list user [" + userId + "] " + " orders");
                    try {
                        User& user = trainBoom->user(userId);
                        SENDVEC(user.getOrders(), "order");
                    }
                    HANDLEERR;
                }

                APIHANDLER(getOrderUser) {
                    std::string userId = request.param(":userId").as<std::string>(),
                        orderId = request.param(":orderId").as<std::string>();

                    api_log->log("get user [" + userId + "] " + " order [" + orderId + "]");

                    try {
                        const User& user = trainBoom->user(userId);
                        const Order& order = user.order(orderId);
                        SENDOBJ(order);
                    }
                    HANDLEERR;
                }

                APIHANDLER(getByUsername) {
                    Json json; json.Parse(request.body());

                    api_log->log("get by username");
                    api_log->log("request => " + json.toString());
                    try {
                        const std::string userId = trainBoom->idByUsername(json["username"]);
                        api_log->log("userid: " + userId);
                        Json tmp;
                        tmp["userId"] = userId;
                        SENDJSON(tmp);
                    }
                    HANDLEERR;
                }

                void insertStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    api_log->log("insert station");
                    Json tmp; tmp.Parse(request.body());
                    api_log->log("request => " + tmp.toString());
                    Station station(tmp);
                    try {
                        trainBoom->insertStation(station);
                        station_log->log("insert [" + station.getId() + ":" + station.getName() + "]");

                        sendJson(response, station.toJson());
                    }
                    HANDLEERR;
                }

                void listStations(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    api_log->log("list stations");
                    sendJson(response, vec2Json(trainBoom->listStations(), "station"));
                }

                void getStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
                    api_log->log("get station [" + stationId + "]");
                    //                    std::cout << stationId << std::endl;
                    try {
                        const Station& station = trainBoom->station(stationId);
                        sendJson(response, station.toJson());
                    }
                    HANDLEERR;
                }

                void updateStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
                    api_log->log("update station [" + stationId + "]");
                    //                    std::cout << stationId << std::endl;
                    try {
                        Station& station = trainBoom->station(stationId);
                        Json tmp; tmp.Parse(request.body());
                        api_log->log(tmp.toString());
                        station.update(tmp);
                        station_log->log("update [" + station.getId() + ":" + station.getName() + "]");
                        sendJson(response, station.toJson());
                    }
                    HANDLEERR;
                }

                APIHANDLER(deleteStation) {
                    std::string stationId = request.param(":stationId").as<std::string>();
                    api_log->log("delete station [" + stationId + "]");
                    try {
                        Station& station = trainBoom->station(stationId);
                        trainBoom->deleteStation(stationId);
                        station_log->log("delete [" + station.getId() + ":" + station.getName() + "]");
                        SENDSUCC("delete station succeeded!");
                    }
                    HANDLEERR;
                }

                APIHANDLER(getByStationName) {
                    api_log->log("get by station name");
                    Json json; json.Parse(request.body());
                    api_log->log("request => " + json.toString());
                    try {
                        std::string stationName = json["name"];
                        std::string stationId = trainBoom->idByStationName(stationName);
                        api_log->log("stationId: " + stationId);
                        Json tmp;
                        tmp["stationId"] = stationId;
                        SENDJSON(tmp);
                    }
                    HANDLEERR;
                }
                /*
                   void addRouteStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                   std::string stationId = request.param(":stationId").as<std::string>();
                   Json json; json.Parse(request.body());
                   try {
                   Station& station = trainBoom->station(stationId);
                   try {
                   station.add(json["stationId"].as<std::string>(), Datetime::parse(json["date"].as<std::string>()), json["routeId"].as<std::string>());
                   sendJson(response, success("Add Route succeeded!"));
                   }
                   catch (...) {
                   sendJson(response, error("Add Route failed!"));
                   }
                   }
                   HANDLEERR;
                   }

                   void delRouteStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                   std::string stationId = request.param(":stationId").as<std::string>();
                   Json json; json.Parse(request.body());
                   try {
                   Station& station = trainBoom->station(stationId);
                   try {
                   station.del(json["stationId"].as<std::string>(), Datetime::parse(json["date"].as<std::string>()), json["routeId"].as<std::string>());
                   sendJson(response, success("Delete Route successed!"));
                   }
                   catch (...) {
                   sendJson(response, error("Delete Route failed!"));
                   }
                   }
                   HANDLEERR;
                   }
                 */
                void queryRouteStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    //                    std::cout << "Done Routing." << std::endl;
                    std::string stationId = request.param(":stationId").as<std::string>();
                    api_log->log("query station [" + stationId + "] route");
                    Json json; json.Parse(request.body());
                    api_log->log("request => " + json.toString());
                    //                   std::cout << "Done Parse." << std::endl;
                    try {
                        Station& station = trainBoom->station(stationId);
                        if (!json.HasMember("routeName")) {
                            const auto& vec = station.query(json["stationName"].as<std::string>());
                            //                     std::cout << "Done query." << std::endl;
                            sendJson(response, vec2Json(vec, "route"));
                        }
                        else {
                            const auto& route = station.query(json["stationName"].as<std::string>(), json["routeName"].as<std::string>());
                            SENDJSON(route.toJson());
                        }
                    }
                    HANDLEERR;
                }

                void insertRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    api_log->log("insert route");
                    try {
                        Json tmp; tmp.Parse(request.body());
                        api_log->log("request => " + tmp.toString());
                        Route route(tmp);
                        trainBoom->insertRoute(route);
                        route_log->log("insert route [" + route.getId() + ":" + route.getName() + "]");

                        sendJson(response, route.toJson());
                    }
                    HANDLEERR;
                }

                APIHANDLER(deleteRoute) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("delete route [" + routeId + "]");
                    try {
                        Route& route = trainBoom->route(routeId);
                        trainBoom->deleteRoute(routeId);
                        route_log->log("delete route [" + route.getId() + ":" + route.getName() + "]");
                        SENDSUCC("delete route succeeded!");
                    }
                    HANDLEERR;
                }

                APIHANDLER(updateRoute) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("update route [" + routeId + "]");
                    try {
                        Json tmp; tmp.Parse(request.body());
                        api_log->log(tmp.toString());
                        trainBoom->deleteRoute(routeId);
                        Route route(tmp);
                        trainBoom->insertRoute(route);

                        route_log->log("update route [" + route.getId() + ":" + route.getName() + "]");

                        sendJson(response, route.toJson());
                    }
                    HANDLEERR;
                }

                APIHANDLER(getByRouteName) {
                    api_log->log("get by route name");
                    Json json; json.Parse(request.body());
                    api_log->log("request => " + json.toString());
                    try {
                        std::string routeName = json["name"];
                        std::string routeId = trainBoom->idByRouteName(routeName);
                        api_log->log("routeId: " + routeId);
                        Json tmp;
                        tmp["routeId"] = routeId;
                        SENDJSON(tmp);
                    }
                    HANDLEERR;
                }

                void listRoutes(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    api_log->log("list routes");
                    sendJson(response, vec2Json(trainBoom->listRoutes(), "route"));
                }

                void getRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("get route [" + routeId + "]");
                    try {
                        Route& route = trainBoom->route(routeId);
                        sendJson(response, route.toJson());
                    }
                    HANDLEERR;
                }

 /*               void startRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    try {
                        Route& route = trainBoom->route(routeId);
                        try {
                            trainBoom->startRoute(route);
                            sendJson(response, success("Start Route succeeded!"));
                        }
                        catch (const exception& e) {
                            sendJson(response, error(e.what()));
                        }
                    }
                    HANDLEERR;
                }

                void stopRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    try {
                        Route& route = trainBoom->route(routeId);
                        try {
                            trainBoom->stopRoute(route);
                            sendJson(response, success("Stop Route succeeded!"));
                        }
                        catch (const exception& e) {
                            sendJson(response, error(e.what()));
                        }
                    }
                    HANDLEERR;
                }
*/
                void queryTicketsRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("query route [" + routeId + "] tickets");
                    Json json; json.Parse(request.body());
                    api_log->log("request => " + json.toString());
		    // std::cout << "Query " << std::endl;
		    // std::cout << json.toString() << std::endl;
                    try {
                        Route& route = trainBoom->route(routeId);
                        Datetime date = Datetime::parse(json["date"].as<std::string>());
                        unsigned l = json["l"].as<unsigned>(), r = json["r"].as<unsigned>();
                        const Segment& segment = route.queryTickets(date, l, r);
                        Json ret = segment.toJson();
                        Information startStation = route.information(l);
                        Information endStation = route.information(r);
                        Duration dayShift = startStation.getLeaveTime().setToDay();
                        startStation.shift(dayShift);
                        endStation.shift(dayShift);
                        ret["startStation"] = startStation.toJson(date);
                        ret["endStation"] = endStation.toJson(date);
                        sendJson(response, ret);
                    }
                    HANDLEERR;
                }

                void bookTicketsRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("book route [" + routeId + "] tickets");
                    try {
                        Json json; json.Parse(request.body());
                        api_log->log("request => " + json.toString());
			// std::cout << "Book " << std::endl;
			// std::cout << json.toString() << std::endl;
                        Route& route = trainBoom->route(routeId);
                        Datetime date = Datetime::parse(json["date"].as<std::string>());
                        std::string userId = json["userId"].as<std::string>();
                        User& user = trainBoom->user(userId);

                        unsigned l = json["l"].as<unsigned>();
                        unsigned r = json["r"].as<unsigned>();
                        std::string ticketType = json["ticketType"].as<std::string>();
                        unsigned ticketNumber = json["ticketNumber"].as<unsigned>();

                        Order order = route.bookTickets(
                                date,
                                l,
                                r,
                                ticketType,
                                ticketNumber
                                );

                        route_log->log("book tickets [" + ticketType + " * " + std::to_string(ticketNumber) + "] / " + route.information(l).getStationName() + " -> " + route.information(r).getStationName());

                        if (!json.HasMember("attach") ||
                                json["attach"].as<bool>()) {
                            user.addOrder(order);

                            user_log->log("user [" + user.getId() + ":" + user.getUsername() + "] attach order [" + order.getId() + "]");

                            sendJson(response, order.toJson());
                        }
                        else {
                            SENDSUCC("book tickets succeeded!");
                        }
                    }
                    catch (const exception& e) {
                        sendJson(response, error(e.what()));
                    }
                }

                void refundTicketsRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    api_log->log("refund route [" + routeId + "] tickets");
                    try {
                        Json json; json.Parse(request.body());
                        api_log->log("request => " + json.toString());

                        Route& route = trainBoom->route(routeId);
                        Datetime date = Datetime::parse(json["date"].as<std::string>());
                        std::string userId = json["userId"].as<std::string>();
                        User& user = trainBoom->user(userId);

                        unsigned l = json["l"].as<unsigned>();
                        unsigned r = json["r"].as<unsigned>();
                        std::string ticketType = json["ticketType"].as<std::string>();
                        unsigned ticketNumber = json["ticketNumber"].as<unsigned>();

                        route.refundTickets(
                                date,
                                l,
                                r,
                                ticketType,
                                ticketNumber
                                );

                        route_log->log("refund tickets [" + ticketType + " * " + std::to_string(ticketNumber) + "] / " + route.information(l).getStationName() + " -> " + route.information(r).getStationName());

                        SENDSUCC("refund tickets succeeded!");
                    }
                    catch (const exception& e) {
                        sendJson(response, error(e.what()));
                    }
                }

                APIHANDLER(getRouteInterval) {
                    api_log->log("get route interval");
                    try {
                        Json json; json.Parse(request.body());
                        api_log->log("request => " + json.toString());

                        std::string startStationId = trainBoom->idByStationName(json["startStation"]);
                        std::string endStationId = trainBoom->idByStationName(json["endStation"]);
                        const Station& station = trainBoom->station(startStationId);
                        if (!json.HasMember("routeName")) {
                            const auto& routes = station.query(json["endStation"]);
                            SENDVEC(routes, "routeInterval");
                        }
                        else {
                            const auto& route = station.query(json["endStation"], json["routeName"].as<std::string>());
                            SENDJSON(route.toJson());
                        }
                    }
                    HANDLEERR;
                }
                /*
                   APIHANDLER(queryRouteInterval) {
                   try {
                   Json json; json.Parse(request.body());
                   Route& route = trainBoom->route(json["routeId"].as<std::string>());
                   unsigned l = json["l"].as<unsigned>(), r = json["r"].as<unsigned>();
                   Json ret("routeIntervalInformation");
                   ret["startStation"] = route.information(l);
                   ret["endStation"] = route.information(r);
                   ret["tickets"] = route.queryTickets(l, r);
                   SENDJSON(ret);
                   }
                   HANDLEERR;
                   }

                   APIHANDLER(queryRouteInterval) {
                   try {
                   Json json; json.Parse(request.body());
                   Route& route = trainBoom->route(json["routeId"].as<std::string>());
                   unsigned l = json["l"].as<unsigned>(), r = json["r"].as<unsigned>();
                   Json ret("routeIntervalInformation");
                   ret["startStation"] = route.information(l);
                   ret["endStation"] = route.information(r);
                   ret["tickets"] = route.queryTickets(l, r);
                   SENDJSON(ret);
                   }
                   HANDLEERR;
                   }
                 */

                typedef std::mutex Lock;
                typedef std::lock_guard<Lock> Guard;
                Lock metricsLock;

                util::stupid_ptr<Net::Http::Endpoint> httpEndpoint;
                Rest::Router router;
                util::stupid_ptr<TrainBoom> trainBoom;
                util::stupid_ptr<Log> user_log, route_log, station_log, api_log;
        };

        class APIServer {
            private:
                util::stupid_ptr<StatsEndpoint> stats;
                util::stupid_ptr<TrainBoom> trainBoom;
                Net::Port port;
                util::stupid_ptr<Log> user_log, route_log, station_log, api_log;

            public:
                APIServer(util::stupid_ptr<TrainBoom> trainBoom, unsigned portNum,
                    util::stupid_ptr<Log> user_log,
                    util::stupid_ptr<Log> route_log,
                    util::stupid_ptr<Log> station_log,
                    util::stupid_ptr<Log> api_log):
                        trainBoom(trainBoom), port(portNum),
                        user_log(user_log),
                        route_log(route_log),
                        station_log(station_log),
                        api_log(api_log) {
                        }

                void run(int thr = 1) {
                    Net::Address addr(Net::Ipv4::any(), port);

                    std::cout << "Listening on port " << port << std::endl;
                    std::cout << "Cores = " << hardware_concurrency() << std::endl;
                    std::cout << "Using " << thr << " threads" <<std::endl;

                    //                std::cout << trainBoom->getId() << std::endl;

                    stats = util::make_stupid<StatsEndpoint>(addr, trainBoom, user_log, route_log, station_log, api_log);

                    stats->shutdownFlag = false;
                    stats->init(thr);
                    stats->start();

                    while (!stats->shutdownFlag) {
                        //                    sleep(1);
                    }
                    //                std::cout << "!!" << std::endl;

                    stats->shutdown();
                }

        };


    };

}
