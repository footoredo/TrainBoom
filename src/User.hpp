#ifndef TRAINBOOM_TRAIN_HPP
#define TRAINBOOM_TRAIN_HPP

#include "util.hpp"
#include <string>

namespace TrainBoom {

enum Gender{Man=0, Woman=1, Other=2};

class User {
    friend class Password;//TODO

private:
    Id id;
    Password password;
    std::string username,avatar,realname,phone,email,motto;
    Gender gender;//Man, Woman, Other
    bool root;
    util::vector<Order> order;//TODO

public:
	User():gender(Other),root(0), id("User") {}
	// User operator=(const User &t):id(t.id),username(t.username),password(t.password),avatar(t.avatar),realname(t.realname),phone(t.phone),email(t.email),motto(t.motto),gender(t.gender),root(t.root),order(t.order) {}
	Id getId() {return id;}
	std::string getUsername() {return username;}
	Password getPassword() {return password;}//eererewre
	std::string getAvatar() {return avatar;}
	std::string getRealname() {return realname;}
	std::string getPhone() {return phone;}
	std::string getEmail() {return email;}
	std::string getMotto() {return motto;}
	Gender getGender() {return gender;}
	bool getRoot() {return root;}
	util::vector<Order> getOrder() {return order;}
	void modifyPassword(Password t)() {password=t;}
	void modifyUsername(str::string t)() {username=t;}
	void modifyAvatar(str::string t)() {avatar=t;}
	void modifyRealname(str::string t)() {realname=t;}
	void modifyPhone(str::string t)() {phone=t;}
	void modifyEmail(str::string t)() {email=t;}
	void modifyMotto(str::string t)() {motto=t;}
	void modifyGender(Gender t)() {gender=t;}
	void modifyRoot(bool t)() {root=t;}
	void bookTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num) {}
	void refundTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num) {}
	void queryTicket(Station from, Station to, Datetime::Datetime date) {}
	void addRoute(Route route) {}//ROOT
	void delRoute(Id routeID) {}//ROOT
	void startRoutes(Train train, Datetime::Datetime date) {}//ROOT
	void stopRoutes(Train train, Datetime::Datetime date) {} //ROOT
	std::string getUsername(Id userID) {}
	Password getPassword(Id userID) {}//ROOT
	std::string getAvatar(Id userID) {}
	std::string getRealname(Id userID) {}//ROOT
	std::string getPhone(Id userID) {}
	std::string getEmail(Id userID) {}
	Gender getGender(Id userID) {}
	std::string getMotto(Id userID) {}
	bool getRoot(Id userID) {}
	util::vector<Order> getOrder(Id userID) {}
	void getLog(Id userID) {}//ROOT
};

}
