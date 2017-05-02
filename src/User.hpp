#ifndef TRAINBOOM_TRAIN_HPP
#define TRAINBOOM_TRAIN_HPP

#include "util.hpp"
#include <string>

namespace TrainBoom {
	
enum Gender{Man=0, Woman=1, Other=2};

class User {
    friend class Password;//TODO

private:
    id_t id;
    Password password;
    std::string username,avatar,realname,phone,email,motto;
    Gender gender;//Man, Woman, Other
    bool root;
    util::vector<Order> order;//TODO

public:
	User():gender(Other),root(0) {}
	User operator=(const User &t):id(t.id),username(t.username),password(t.password),avatar(t.avatar),realname(t.realname),phone(t.phone),email(t.email),motto(t.motto),gender(t.gender),root(t.root),order(t.order) {}
	id_t getId() {return id;}
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
	void delRoute(size_t routeID) {}//ROOT
	void startRoutes(Train train, Datetime::Datetime date) {}//ROOT
	void stopRoutes(Train train, Datetime::Datetime date) {} //ROOT
	std::string getUsername(size_t userID) {}
	Password getPassword(size_t userID) {}//ROOT
	std::string getAvatar(size_t userID) {}
	std::string getRealname(size_t userID) {}//ROOT
	std::string getPhone(size_t userID) {}
	std::string getEmail(size_t userID) {}
	Gender getGender(size_t userID) {}
	std::string getMotto(size_t userID) {}
	bool getRoot(size_t userID) {}
	util::vector<Order> getOrder(size_t userID) {}
	void getLog(size_t userID) {}//ROOT
};

}
