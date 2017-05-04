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
        }

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
                }

                void _shutdown(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::cout << "Received shutdown." << std::endl;
                    response.send(Http::Code::Ok, "Shutdown.");
//                    shutdown();
                    shutdownFlag = true;
                }

                void insertUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
//                    std::cout << request.body() << std::endl;
/*                    auto query = request.query();
                    std::cout << request.param("username").as<std::string>() << std::endl;
                    Json userJson("user");
                    if (query.has("username")) {
                        userJson["username"] = query.get("username").get();
                        std::cout << "!!!" << " " << userJson["username"].as<std::string>() << std::endl;
                    }
                    if (query.has("avatar")) userJson["avatar"] = query.get("avatar").get();
                    if (query.has("realname")) userJson["realname"] = query.get("realname").get();
                    if (query.has("phone")) userJson["phone"] = query.get("phone").get();
                    if (query.has("email")) userJson["email"] = query.get("email").get();
                    if (query.has("motto")) userJson["motto"] = query.get("motto").get();
                    if (query.has("gender")) userJson["gender"] = std::stoi(query.get("gender").get());
                    if (query.has("isRoot")) userJson["isRoot"] = !!std::stoi(query.get("isRoot").get());*/
//                    std::cout << userJson.toString() << std::endl;
                    try {
                        User user(Json("user").Parse(request.body()));
                        trainBoom->insertUser(user);

                        Generic::sendJson(response, user.toJson());
                    }
                    catch (const User::information_missing& e) {
                        Generic::sendJson(response, Generic::error(e.what()));
                    }
                }

                void listUsers(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    const util::vector<std::string>& users = trainBoom->listUsers();
                    Json usersList("usersList");
                    usersList["users"].SetArray();
                    for (const Id& user: users) {
                        usersList["users"].PushBack(user);
                    }

                    Generic::sendJson(response, usersList);
                }

                void getUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
//                    std::cout << userId << std::endl;
                    try {
                        const User& user = trainBoom->user(userId);
                        Generic::sendJson(response, user.toJson());
                    }
                    catch (const TrainBoom::TrainBoom::id_not_exist& e) {
                        Generic::sendJson(response, Generic::error("UserId not found!"));
                    }
                }

                void updateUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::string userId = request.param(":userId").as<std::string>();
//                    std::cout << userId << std::endl;
                    try {
                        User& user = trainBoom->user(userId);
                        user.update(Json("user").Parse(request.body()));
                        Generic::sendJson(response, user.toJson());
                    }
                    catch (const TrainBoom::TrainBoom::id_not_exist& e) {
                        Generic::sendJson(response, Generic::error("UserId not found!"));
                    }
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

        public:
            APIServer(util::stupid_ptr<TrainBoom> trainBoom, unsigned portNum):
                trainBoom(trainBoom), port(portNum) {}

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
                    sleep(1);
                }

                stats->shutdown();
            }

        };


    };

}
