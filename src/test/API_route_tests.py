import requests

#url = "http://39.108.7.208:3000"
url = "http://localhost:3000"
s = requests.Session()
s.headers.update({"Cache-Control": "no-cache"})

route = {
    "name": "G105",
    "n":2,
    "informations": [
        {
            "stationId": "Chanzhou Bei",
            "distance": 1,
            "flags": 1,
            "leaveTime": "2017/5/5 12:23"
        },
        {
            "stationId": "Shanghai Hongqiao",
            "distance": 2,
            "flags": 2,
            "arriveTime": "2017/5/5 13:15"
        }
    ],
    "segments": [
        {
            "tickets": {
                "first class": {
                    "price": 129.5,
                    "number": 17
                },
                "second class": {
                    "price": 74.5
                },
                "bussiness class": {
                    "price": 239.5,
                    "number": 5
                }
            }
        }
    ]
}

res = s.post(url + "/stations", json = {"name": "Changzhou North"})
import json
route["informations"][0]["stationId"] = stationId0 = res.json()["id"]
res = s.post(url + "/stations", json = {"name": "Shanghai Hong Qiao"})
route["informations"][1]["stationId"] = stationId1 = res.json()["id"]


res = s.post(url + "/routes", json = route);
#print json.dumps(res.json(), indent=4)
routeId0 = res.json()["id"]

res = s.get(url + "/routes/" + routeId0 + "/start");
#print json.dumps(res.json(), indent=4)
assert res.json()["type"] == "success", "Failed to start the route!"
print "Start route check passed!"

res = s.post(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
#print json.dumps(res.json(), indent=4)
result = res.json()["routes"]
assert len(result) == 1 and result[0]["data"]["routeId"] == routeId0, "Query check failed!"
print "1st route test passed!"

res = s.post(url + "/routes", json = route);
routeId1 = res.json()["id"]
res = s.get(url + "/routes/" + routeId1 + "/start");
res = s.post(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
result = res.json()["routes"]
resultRouteId = map(lambda item: item["data"]["routeId"], result)
assert len(result) == 2 and routeId0 in resultRouteId and routeId1 in resultRouteId, "Query check after 2nd route failed!"
print "2nd route test passed!"

res = s.get(url + "/routes/" + routeId0 + "/stop");
assert res.json()["type"] == "success", "Failed to stop the 1st route!"
res = s.post(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
result = res.json()["routes"]
assert len(result) == 1 and result[0]["data"]["routeId"] == routeId1, "Query check after stop 1st route failed!"

res = s.get(url + "/routes/" + routeId0 + "/stop");
#print json.dumps(res.json(), indent=4)
assert res.json()["type"] == "error", "Duplicated stop check failed!"
print "stop test passed!"

res = s.post(url + "/routes/" + routeId1 + "/tickets", json = {
    "l": 0,
    "r": 1
    })

#print json.dumps(res.json(), indent=4)
assert res.json()["tickets"]["first class"]["number"] == 17, "Query tickets check failed!"
print "Query tickets check passed!"

res = s.post(url + "/users", json = {"username": "footoredo", "salt": "iamasalt", "password": "."})
userId = res.json()["id"]

data = {
    "userId": userId,
    "l": 0,
    "r": 1,
    "ticketType": "first class",
    "ticketNumber": 1000
    }

res = s.put(url + "/routes/" + routeId1 + "/tickets", json=data)
#print json.dumps(res.json(), indent=4)
assert res.json()["type"] == "error"
print "Overbook ticket test passed!"
data = {
    "userId": userId,
    "l": 0,
    "r": 1,
    "ticketType": "first class",
    "ticketNumber": 2
    }

res = s.put(url + "/routes/" + routeId1 + "/tickets", json = {
    "userId": userId,
    "l": 0,
    "r": 1,
    "ticketType": "first class",
    "ticketNumber": 2
    })
#print json.dumps(res.json(), indent=4)
assert res.json()["data"]["ticketPrice"] == 259.0
orderJson = res.json()
orderId = res.json()["id"]
print "book ticket test passed!"

res = s.get(url + "/users/" + userId + "/orders");
orders = res.json()["orders"]
assert len(orders) == 1 and orders[0] == orderId, "Order attaching check failed!"

res = s.get(url + "/users/" + userId + "/orders/" + orderId)
#print json.dumps(orderJson, indent=4)
assert res.json() == orderJson, "Order attaching check failed!"

print "Order attaching check passed!"

res = s.post(url + "/routes/" + routeId1 + "/tickets", json = {
    "l": 0,
    "r": 1 
    })
assert res.json()["tickets"]["first class"]["number"] == 15, "Query tickets after booking check failed!"
print "Query tickets after booking check passed!"

print "All tests passed!"
