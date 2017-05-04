#include "TrainBoom.hpp"
#include "util.hpp"

#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"

namespace TrainBoom {

    namespace API {

        using namespace Net;

        bool shutdownFlag;
        class StatsEndpoint {
            public:
                StatsEndpoint(Net::Address addr)
                    : httpEndpoint(util::make_stupid<Net::Http::Endpoint>(addr))
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
                }

                void _shutdown(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    std::cout << "Received shutdown." << std::endl;
                    response.send(Http::Code::Ok, "Shutdown.");
//                    shutdown();
                    shutdownFlag = true;
                }

                void insertUser(const Rest::Request& request, Net::Http::ResponseWriter response) {
                    auto query = request.query();
                    Json userJson("user");
                    if (query.has("username")) userJson["username"] = query.get("username").get();
                    if (query.has("avatar")) userJson["avatar"] = query.get("avatar").get();
                    if (query.has("realname")) userJson["realname"] = query.get("realname").get();
                    if (query.has("phone")) userJson["phone"] = query.get("phone").get();
                    if (query.has("email")) userJson["email"] = query.get("email").get();
                    if (query.has("motto")) userJson["motto"] = query.get("motto").get();
                    if (query.has("gender")) userJson["gender"] = std::stoi(query.get("gender").get());
                    if (query.has("isRoot")) userJson["isRoot"] = !!std::stoi(query.get("isRoot").get());
                    std::cout << userJson.toString() << std::endl;
                    User user(userJson);
                    response.send(Http::Code::Created, user.getId());
                }


                typedef std::mutex Lock;
                typedef std::lock_guard<Lock> Guard;
                Lock metricsLock;

                util::stupid_ptr<Net::Http::Endpoint> httpEndpoint;
                Rest::Router router;
        };

        class APIServer {
        private:
            util::stupid_ptr<StatsEndpoint> stats;
            util::stupid_ptr<TrainBoom> trainBoom;
            Net::Port port;

        public:
            APIServer(util::stupid_ptr<TrainBoom> trainBoom, unsigned portNum):
                trainBoom(trainBoom), port(portNum) {}

            void run(int thr = 2) {
                Net::Address addr(Net::Ipv4::any(), port);

                std::cout << "Listening on port " << port << std::endl;
                std::cout << "Cores = " << hardware_concurrency() << std::endl;
                std::cout << "Using " << thr << " threads" <<std::endl;

                stats = util::make_stupid<StatsEndpoint>(addr);

                shutdownFlag = false;
                stats->init(thr);
                stats->start();

                while (!shutdownFlag) {
                    sleep(1);
                }

                stats->shutdown();
            }

        };


    };

}
