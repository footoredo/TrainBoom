# `User`

## Introduction

��ͨ�û����ࡣ 

## Members

Name|Type|Description
--|--|--
`ID` | size_t | �û���� 
`username` | `std::string` | �û���
`password` | `Password` | ����(Ĭ��0000) 
`avatar` | `std::string` | ͷ����ַ
`realname` | `std::string` | ��ʵ���� 
`phone` | std::string | �ֻ�����
`email` | std::string | �����ַ 
`gender` | `Gender` | �Ա� 
`motto` | `std::string` | ����ǩ�� 
`root` | `bool` | �Ƿ��ǹ���Ա 
`order` | `util::vector<Order>` | ����
 
## Member classes

Name|Description
--|--
[`Password`](User/Password.md)|�������  


## Member functions

Name|Description
--|--
`(constructor)`|���캯��
`(destructor)`|��������
`operator=`|��ֵ����
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