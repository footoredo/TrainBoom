import requests

url = "http://localhost:3000"

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

res = requests.post(url + "/stations", json = {"name": "Changzhou Bei"})
route["informations"][0]["stationId"] = stationId0 = res.json()["id"]
res = requests.post(url + "/stations", json = {"name": "Shanghai Hong Qiao"})
route["informations"][1]["stationId"] = stationId1 = res.json()["id"]

res = requests.post(url + "/routes", json = route);
routeId0 = res.json()["id"]
res = requests.get(url + "/routes/" + routeId0 + "/start");
assert res.json()["type"] == "success", "Failed to start the route!"

res = requests.get(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
result = res.json()["data"]["routes"]
assert len(result) == 1 and result[0] == routeId0, "Query check failed!"
print "1st route test passed!"

res = requests.post(url + "/routes", json = route);
routeId1 = res.json()["id"]
res = requests.get(url + "/routes/" + routeId1 + "/start");
res = requests.get(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
result = res.json()["data"]["routes"]
assert len(result) == 2 and routeId0 in result and routeId1 in result, "Query check after 2nd route failed!"
print "2nd route test passed!"

res = requests.get(url + "/routes/" + routeId0 + "/stop");
assert res.json()["type"] == "success", "Failed to stop the 1st route!"
res = requests.get(url + "/stations/" + stationId0 + "/routes", json = {
    "stationId": stationId1,
    "date": "2017/5/5"
    })
result = res.json()["data"]["routes"]
assert len(result) == 1 and result[0] == routeId1, "Query check after stop 1st route failed!"

res = requests.get(url + "/routes/" + routeId0 + "/stop");
assert res.json()["type"] == "error", "Duplicated stop check failed!"
print "stop test passed!"

res = requests.get(url + "/routes/" + routeId1 + "/tickets", json = {
    "startStation": stationId0,
    "endStation": stationId1
    })
assert res.json()["data"]["tickets"]["first class"]["data"]["number"] == 17, "Query tickets check failed!"
print "Query tickets check passed!"

res = requests.post(url + "/users", json = {"username": "footoredo", "salt": "iamasalt", "password": "."})
userId = res.json()["id"]

res = requests.put(url + "/routes/" + routeId1 + "/tickets", json = {
    "userId": userId,
    "startStationId": stationId0,
    "endStationId": stationId1,
    "ticketType": "first class",
    "ticketNumber": 1000
    })
assert res.json()["type"] == "error"
print "Overbook ticket test passed!"

res = requests.put(url + "/routes/" + routeId1 + "/tickets", json = {
    "userId": userId,
    "startStationId": stationId0,
    "endStationId": stationId1,
    "ticketType": "first class",
    "ticketNumber": 2
    })
assert res.json()["data"]["ticketPrice"] == 259.0
print "book ticket test passed!"

res = requests.get(url + "/routes/" + routeId1 + "/tickets", json = {
    "startStation": stationId0,
    "endStation": stationId1
    })
assert res.json()["data"]["tickets"]["first class"]["data"]["number"] == 15, "Query tickets after booking check failed!"
print "Query tickets after booking check passed!"

print "All tests passed!"
