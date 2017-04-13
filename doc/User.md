# `User`

## Introduction

普通用户的类。 

## Members

Name|Type|Description
--|--|--
`ID` | size_t | 用户编号 
`username` | `std::string` | 用户名
`password` | `Password` | 密码(默认0000) 
`avatar` | `std::string` | 头像网址
`realname` | `std::string` | 真实姓名 
`phone` | std::string | 手机号码
`email` | std::string | 邮箱地址 
`gender` | `Gender` | 性别 
`motto` | `std::string` | 个人签名 
`root` | `bool` | 是否是管理员 
`order` | `util::vector<Order>` | 订单
 
## Member classes

Name|Description
--|--
[`Password`](User/Password.md)|密码的类  


## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`operator=`|赋值函数
`getID` |
`getUsername` |
`getPassword` |
`getAvatar` |
`getRealname` |
`getPhone` |
`getEmail` |
`getGender` |
`getMotto` |
`getRoot` |
`getOrder` |
`modifyUsername` |
`modifyPassword` |
`modifyAvatar` |
`modifyRealname` |
`modifyPhone` |
`modifyEmail` |
`modifyGender` |
`modifyMotto` |
`modifyRoot` |
`bookTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num)` |
`refundTicket(Train train, Station from, Station to, size_t lowPrice, size_t highPrice, int num)` |
`queryTicket(Station from, Station to, Datetime::Datetime date)` |
`addRoute(Route route)` | root only
`delRoute(size_t routeID)` | root only
`startRoutes(Train train, Datetime::Datetime date)` | root only
`stopRoutes(Train train, Datetime::Datetime date)` | root only
`getUsername(size_t userID)` |
`getPassword(size_t userID)` | root only
`getAvatar(size_t userID)` |
`getRealname(size_t userID)` | root only
`getPhone(size_t userID)` |
`getEmail(size_t userID)` |
`getGender(size_t userID)` |
`getMotto(size_t userID)` |
`getRoot(size_t userID)` |
`getOrder(size_t userID)` |
`getLog` | root only