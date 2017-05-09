#include "TrainBoom.hpp"
#include "util.hpp"

#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"

namespace TrainBoom {

    namespace API {

        using namespace Net;

        namespace Generic {
            void sendJson(Net::Http::ResponseWriter& response, const Json& json) {
                using namespace Net::Http;
                response.headers()
                    .add<Header::ContentType>(MIME(Application, Json));

                auto stream = response.stream(Net::Http::Code::Ok);
                stream << json.toString().c_str() << ends;
            }

            Json error(const std::string& errMsg) {
                Json err("error"); err["errMsg"] = errMsg;
                return err;
            }

            Json success(const std::string& succMsg) {
                Json succ("success"); succ["succMsg"] = succMsg;
                return succ;
            }

            Json vec2Json(const util::vector<std::string>& vec, const std::string type) {
                Json json(type + "sList");
                json[type + "s"].SetArray();
                for (const std::string& item: vec)
                    json[type + "s"].PushBack(item);
                return json;
            }

            template <class T>
            Json vec2Json(const util::vector<T>& vec, const std::string type) {
                Json json(type + "sList");
                json[type + "s"].SetArray();
                for (const T& item: vec)
                    json[type + "s"].PushBack(item.toJson());
                return json;
            }
        }
#define SENDJSON(json) Generic::sendJson(response, json)
#define SENDOBJ(obj) SENDJSON(obj.toJson())
#define SENDVEC(vec, type) SENDJSON(Generic::vec2Json(vec, type))
#define SENDSUCC(succMsg) Generic::sendJson(response, Generic::success(succMsg))
#define SENDERR(errMsg) Generic::sendJson(response, Generic::error(errMsg))
#define APIHANDLER(APIName) void APIName(const Rest::Request& request, Net::Http::ResponseWriter response)
#define ROUTING(method, path, func) Routes::method(router, path, Routes::bind(&StatsEndpoint::func, this))
#define HANDLEERR catch (const exception& e) { SENDERR(e.what()); }

        class StatsEndpoint {
            public:
                bool shutdownFlag;
                StatsEndpoint(Net::Address addr, const util::stupid_ptr<TrainBoom>& trainBoom)
                    : httpEndpoint(util::make_stupid<Net::Http::Endpoint>(addr)), trainBoom(trainBoom)
                { }

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

                    Routes::Get(router, "/shutdown", Routes::bind(&StatsEndpoint::_shutdown, this));

                    Routes::Post(router, "/users", Routes::bind(&StatsEndpoint::insertUser, this));
                    Routes::Get(router, "/users", Routes::bind(&StatsEndpoint::listUsers, this));
                    Routes::Get(router, "/users/:userId", Routes::bind(&StatsEndpoint::getUser, this));
                    Routes::Put(router, "/users/:userId", Routes::bind(&StatsEndpoint::updateUser, this));
                    ROUTING(Get, "/users/:userId/orders", listOrdersUser);
                    ROUTING(Get, "/users/:userId/orders/:orderId", getOrderUser);
                    ROUTING(Post, "/users/username", getByUsername);
              //      Routes::Get(router, "/users/:userId/orders", Routes::bind(&StatsEndpoint::listOrdersUser, this));
                //    Routes::Get(router, "/users/:userId/orders/:orderId", Routes::bind(&StatsEndpoint::getOrderUser, this));

                    Routes::Post(router, "/stations", Routes::bind(&StatsEndpoint::insertStation, this));
                    Routes::Get(router, "/stations", Routes::bind(&StatsEndpoint::listStations, this));
                    Routes::Get(router, "/stations/:stationId", Routes::bind(&StatsEndpoint::getStation, this));
                    Routes::Put(router, "/stations/:stationId", Routes::bind(&StatsEndpoint::updateStation, this));
                    ROUTING(Post, "/stations/name", getByStationName);
                    // Routes::Post(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::addRouteStation, this));
                    // Routes::Delete(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::delRouteStation, this));
                    Routes::Post(router, "/stations/:stationId/routes", Routes::bind(&StatsEndpoint::queryRouteStation, this));

                    Routes::Post(router, "/routes", Routes::bind(&StatsEndpoint::insertRoute, this));
                    Routes::Get(router, "/routes", Routes::bind(&StatsEndpoint::listRoutes, this));
                    Routes::Get(router, "/routes/:routeId", Routes::bind(&StatsEndpoint::getRoute, this));
//                    Routes::Put(router,)
                    Routes::Get(router, "/routes/:routeId/start", Routes::bind(&StatsEndpoint::startRoute, this));
                    Routes::Get(router, "/routes/:routeId/stop", Routes::bind(&StatsEndpoint::stopRoute, this));
                    Routes::Post(router, "/routes/:routeId/tickets", Routes::bind(&StatsEndpoint::queryTicketsRoute, this));
                    Routes::Put(router, "/routes/:routeId/tickets", Routes::bind(&StatsEndpoint::bookTicketsRoute, this));
                    
                    ROUTING(Post, "/queryRoute", queryRoute);
                }

                void _shutdown(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::cout << "Received shutdown." << std::endl;
                    response.send(Http::Code::Ok, "Shutdown.");
//                    shutdown();
                    shutdownFlag = true;
                }

                void insertUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    try {
                        User user(Json("user").Parse(request.body()));
                        trainBoom->insertUser(user);

                        Generic::sendJson(response, user.toJson());
                    }
		    HANDLEERR;
                }

                void listUsers(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    Generic::sendJson(response, Generic::vec2Json(trainBoom->listUsers(), "user"));
                }

                void getUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
//                    std::cout << userId << std::endl;
                    try {
                        const User& user = trainBoom->user(userId);
                        Generic::sendJson(response, user.toJson());
                    }
		    HANDLEERR;
                }

                void updateUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
//                    std::cout << userId << std::endl;
                    try {
                        User& user = trainBoom->user(userId);
                        try {
                            user.update(Json("user").Parse(request.body()));
                            Generic::sendJson(response, user.toJson());
                        }
                        catch (const User::information_missing& e) {
                            Generic::sendJson(response, Generic::error(e.what()));
                        }
                    }
		    HANDLEERR;
                }

                APIHANDLER(listOrdersUser) {
                    std::string userId = request.param(":userId").as<std::string>();
                    try {
                        User& user = trainBoom->user(userId);
                        SENDVEC(user.getOrders(), "order");
                    }
                    HANDLEERR;
                }

                APIHANDLER(getOrderUser) {
                    std::string userId = request.param(":userId").as<std::string>(),
                        orderId = request.param(":orderId").as<std::string>();

                    try {
                        const User& user = trainBoom->user(userId);
                        const Order& order = user.order(orderId);
                        SENDOBJ(order);
                    }
                    HANDLEERR;
                }

                APIHANDLER(getByUsername) {
                    Json json; json.Parse(request.body());
                    try {
                        const std::string userId = trainBoom->idByUsername(json["username"]);
                        Json json("userId");
                        json["userId"] = userId;
                        SENDJSON(json);
                    }
                    HANDLEERR;
                }

                void insertStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    Station station(Json("station").Parse(request.body()));
                    try {
                    trainBoom->insertStation(station);

                    Generic::sendJson(response, station.toJson());
                    }
                    HANDLEERR;
                }

                void listStations(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    Generic::sendJson(response, Generic::vec2Json(trainBoom->listStations(), "station"));
                }

                void getStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
//                    std::cout << stationId << std::endl;
                    try {
                        const Station& station = trainBoom->station(stationId);
                        Generic::sendJson(response, station.toJson());
                    }
		    HANDLEERR;
                }

                void updateStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
//                    std::cout << stationId << std::endl;
                    try {
                        Station& station = trainBoom->station(stationId);
                        station.update(Json("station").Parse(request.body()));
                        Generic::sendJson(response, station.toJson());
                    }
		    HANDLEERR;
                }

                APIHANDLER(getByStationName) {
                    try {
                        std::string stationName = Json().Parse(request.body())["name"];
                        std::string stationId = trainBoom->idByStationName(stationName);
                        Json tmp("stationId");
                        tmp["stationId"] = stationId;
                        SENDJSON(tmp);
                    }
                    HANDLEERR;
                }

                void addRouteStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
                    Json json; json.Parse(request.body());
                    try {
                        Station& station = trainBoom->station(stationId);
                        try {
                            station.add(json["stationId"].as<std::string>(), Datetime::parse(json["date"].as<std::string>()), json["routeId"].as<std::string>());
                            Generic::sendJson(response, Generic::success("Add Route succeeded!"));
                        }
                        catch (...) {
                            Generic::sendJson(response, Generic::error("Add Route failed!"));
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
                            Generic::sendJson(response, Generic::success("Delete Route successed!"));
                        }
                        catch (...) {
                            Generic::sendJson(response, Generic::error("Delete Route failed!"));
                        }
                    }
		    HANDLEERR;
                }

                void queryRouteStation(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string stationId = request.param(":stationId").as<std::string>();
                    Json json; json.Parse(request.body());
                    try {
                        Station& station = trainBoom->station(stationId);
                        const auto& vec = station.query(json["stationId"].as<std::string>(), Datetime::parse(json["date"].as<std::string>()));
                        Generic::sendJson(response, Generic::vec2Json(vec, "route"));
                    }
		    HANDLEERR;
                }

                void insertRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    try {
                        Route route(Json("route").Parse(request.body()));
                        trainBoom->insertRoute(route);

                        Generic::sendJson(response, route.toJson());
                    }
		    HANDLEERR;
                }

                void listRoutes(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    Generic::sendJson(response, Generic::vec2Json(trainBoom->listRoutes(), "route"));
                }

                void getRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    try {
                        Route& route = trainBoom->route(routeId);
                        Generic::sendJson(response, route.toJson());
                    }
		    HANDLEERR;
                }

                void startRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    try {
                        Route& route = trainBoom->route(routeId);
                        try {
                            trainBoom->startRoute(route);
                            Generic::sendJson(response, Generic::success("Start Route succeeded!"));
                        }
                        catch (const exception& e) {
                            Generic::sendJson(response, Generic::error(e.what()));
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
                            Generic::sendJson(response, Generic::success("Stop Route succeeded!"));
                        }
                        catch (const exception& e) {
                            Generic::sendJson(response, Generic::error(e.what()));
                        }
                    }
		    HANDLEERR;
                }

                void queryTicketsRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string routeId = request.param(":routeId").as<std::string>();
                    Json json; json.Parse(request.body());
                    try {
                        Route& route = trainBoom->route(routeId);
                        try {
                            const Segment& segment = route.queryTickets(json["startStation"].as<std::string>(), json["endStation"].as<std::string>());
                            Generic::sendJson(response, segment.toJson());
                        }
                        catch (const exception& e) {
                            Generic::sendJson(response, Generic::error(e.what()));
                        }
                    }
		    HANDLEERR;
                }

                void bookTicketsRoute(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    try {
                        Json json; json.Parse(request.body());
                        std::string routeId = request.param(":routeId").as<std::string>();
                        Route& route = trainBoom->route(routeId);
                        std::string userId = json["userId"].as<std::string>();
                        User& user = trainBoom->user(userId);

                        Order order = route.bookTickets(
                                json["startStationId"].as<std::string>(),
                                json["endStationId"].as<std::string>(),
                                json["ticketType"].as<std::string>(),
                                json["ticketNumber"].as<unsigned>()
                                );
                        user.addOrder(order);
                        Generic::sendJson(response, order.toJson());
                    }
                    catch (const exception& e) {
                        Generic::sendJson(response, Generic::error(e.what()));
                    }
                }

                APIHANDLER(queryRoute) {
                    try {
                        Json json; json.Parse(request.body());
                        std::string startStationId = trainBoom->idByStationName(json["startStation"]);
                        std::string endStationId = trainBoom->idByStationName(json["endStation"]);
                        const Station& station = trainBoom->station(startStationId);
                        const auto& routes = station.query(endStationId, Datetime::parse(json["date"]));
                        Json ret("queryResult");
                        ret["list"].SetArray();
                        for (const auto& routeId: routes) {
                            Json item;
                            item["routeId"] = routeId;
                            item["tickets"] = trainBoom->route(routeId).queryTickets(startStationId, endStationId).toJson();
                            ret["list"].PushBack(item);
                        }
                        SENDJSON(ret);
                    }
                    HANDLEERR;
                }

                typedef std::mutex Lock;
                typedef std::lock_guard<Lock> Guard;
                Lock metricsLock;

                util::stupid_ptr<Net::Http::Endpoint> httpEndpoint;
                Rest::Router router;
                util::stupid_ptr<TrainBoom> trainBoom;
        };

        class APIServer {
        private:
            util::stupid_ptr<StatsEndpoint> stats;
            util::stupid_ptr<TrainBoom> trainBoom;
            Net::Port port;
	    std::string csvFile;

        public:
            APIServer(util::stupid_ptr<TrainBoom> trainBoom, unsigned portNum, std::string csvFile = ""):
                trainBoom(trainBoom), port(portNum), csvFile(csvFile) {
			if (csvFile != "") {
				trainBoom->loadFromCSV(csvFile);
			}
		}

            void run(int thr = 1) {
                Net::Address addr(Net::Ipv4::any(), port);

                std::cout << "Listening on port " << port << std::endl;
                std::cout << "Cores = " << hardware_concurrency() << std::endl;
                std::cout << "Using " << thr << " threads" <<std::endl;

//                std::cout << trainBoom->getId() << std::endl;

                stats = util::make_stupid<StatsEndpoint>(addr, trainBoom);

                stats->shutdownFlag = false;
                stats->init(thr);
                stats->start();

                while (!stats->shutdownFlag) {
//                    sleep(1);
                }

                stats->shutdown();
            }

        };


    };

}
