#ifndef TRAINBOOM_USER_HPP
#define TRAINBOOM_USER_HPP

#include "util.hpp"
#include <string>
#include <vector>

namespace TrainBoom {

enum Gender{Man=0, Woman=1, Other=2};

class User {
    //friend class Password;//TODO

    typedef std::string Password;
    typedef std::string Order;

private:
    Password password;
    std::string username,avatar,realname,phone,email,motto;
    Gender gender;//Man, Woman, Other
    bool isRoot;
    std::vector<Id> order;//TODO
    Id id;

public:
	User():gender(Other),isRoot(0), id("User") {}
	// User operator=(const User &t):id(t.id),username(t.username),password(t.password),avatar(t.avatar),realname(t.realname),phone(t.phone),email(t.email),motto(t.motto),gender(t.gender),root(t.root),order(t.order) {}
	Id getId() {return id;}
	std::string getUsername() {return username;}
	// Password getPassword() {return password;}//eererewre
    bool verifyPassword(const std::string& _password) {
        return true;
    }
	std::string getAvatar() {return avatar;}
	std::string getRealname() {return realname;}
	std::string getPhone() {return phone;}
	std::string getEmail() {return email;}
	std::string getMotto() {return motto;}
	Gender getGender() {return gender;}
	bool getIsRoot() {return isRoot;}
	std::vector<Id> getOrder() {return order;}
	void modifyPassword(Password t) {password=t;}
	void modifyUsername(std::string t) {username=t;}
	void modifyAvatar(std::string t) {avatar=t;}
	void modifyRealname(std::string t) {realname=t;}
	void modifyPhone(std::string t) {phone=t;}
	void modifyEmail(std::string t) {email=t;}
	void modifyMotto(std::string t) {motto=t;}
	void modifyGender(Gender t) {gender=t;}
	void modifyIsRoot(bool t) {isRoot=t;}
    void addOrder(const Id& orderId) {
        order.push_back(orderId);
    }
	/*void bookTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num) {}
	void refundTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num) {}
	void queryTicket(Station from, Station to, Datetime::Datetime date) {}
	void addRoute(Route route) {}//ROOT
	void delRoute(Id routeID) {}//ROOT
	void startRoutes(Train train, Datetime::Datetime date) {}//ROOT
	void stopRoutes(Train train, Datetime::Datetime date) {} //ROOT
	std::string getUsername(Id UserId) {}
	Password getPassword(Id UserId) {}//ROOT
	std::string getAvatar(Id UserId) {}
	std::string getRealname(Id UserId) {}//ROOT
	std::string getPhone(Id UserId) {}
	std::string getEmail(Id UserId) {}
	Gender getGender(Id UserId) {}
	std::string getMotto(Id UserId) {}
	bool getRoot(Id UserId) {}
	std::vector<Id> getOrder(Id UserId) {}
	void getLog(Id UserId) {}//ROOT*/

    util::Json toJson() const {
        util::Json json("user", id);
        json["username"] = username;
        json["avatar"] = avatar;
        json["realname"] = realname;
        json["phone"] = phone;
        json["email"] = email;
        json["motto"] = motto;
        json["gender"] = gender;
        json["isRoot"] = isRoot;
        return json;
    }

    util::Json getOrderJson() const {
        util::Json json("order");
        json["UserId"] = id;
        json["order"].SetArray();
        for (const auto& item: order) {
            json["order"].PushBack(item);
        }
        return json;
    }
};

}

#endif