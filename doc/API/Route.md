# Route

## insert

### HTTP Request

```
POST /routes
```

### Request body

```json
{
    "informations": [
        {
            "distance": 1,
            "stationId": "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
            "leaveTime": "2017/5/5 12:23",
            "flags": 1
        },
        {
            "arriveTime": "2017/5/5 13:15",
            "distance": 2,
            "stationId": "da81e4553f89751731dea2adb486a4582ba3fa1d",
            "flags": 2
        }
    ],
    "segments": [
        {
            "tickets": {
                "second class": {
                    "price": 74.5
                },
                "bussiness class": {
                    "price": 239.5,
                    "number": 5
                },
                "first class": {
                    "price": 129.5,
                    "number": 17
                }
            }
        }
    ],
    "name": "G105",
    "n": 2
}
```

### Response body

```json
{
    "data": {
        "informations": [
            {
                "data": {
                    "distance": 1,
                    "stationId": "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
                    "leaveTime": "2017/5/5 12:23",
                    "flags": 1
                },
                "type": "information",
                "id": "7347174fba44779087fc719630d8b56ac078d1c0"
            },
            {
                "data": {
                    "arriveTime": "2017/5/5 13:15",
                    "distance": 2,
                    "stationId": "da81e4553f89751731dea2adb486a4582ba3fa1d",
                    "flags": 2
                },
                "type": "information",
                "id": "f45edf78549e96124b39dc616fa42c64adb63976"
            }
        ],
        "segments": [
            {
                "data": {
                    "tickets": {
                        "bussiness class": {
                            "data": {
                                "price": 239.5,
                                "number": 5
                            },
                            "type": "attribute"
                        },
                        "second class": {
                            "data": {
                                "price": 74.5,
                                "number": 200
                            },
                            "type": "attribute"
                        },
                        "first class": {
                            "data": {
                                "price": 129.5,
                                "number": 17
                            },
                            "type": "attribute"
                        }
                    }
                },
                "type": "segment",
                "id": "53886ca5ae915b79603630d7efb22dee87675975"
            }
        ],
        "name": "G105",
        "n": 2
    },
    "type": "route",
    "id": "d092e0575781635f2109791936a8d020f68082aa"
}
```

## list

### HTTP Request

```
GET /routes
```

### Response body
```json
{
    "routes": [
        "169993781d665438d3a3cf267187cc07a93c75e6",
        "d0bda205ca3042fd5398c7e2c8bc163803080888"
    ]
}
```

## get

### HTTP Request

```
GET /routes/:routeId
```

### Response body
```
GET /routes/748b74b9dc7bb96a828b72cdd0b8eb492a608eb6
```
```json
{
    "data": {
        "informations": [
            {
                "distance": 1,
                "flags": 1,
                "leaveTime": "2017/5/5 12:23",
                "stationId": "18f3b9f27a590702bada96ae323017513ed4bc75"
            },
            {
                "arriveTime": "2017/5/5 13:15",
                "distance": 2,
                "flags": 2,
                "stationId": "ea73227c2842e2ce637dbc8a313372f70d7374d6"
            }
        ],
        "n": 2,
        "name": "G105",
        "running": false,
        "segments": [
            {
                "tickets": {
                    "bussiness class": {
                        "nonstop": false,
                        "number": 5,
                        "price": 239.5
                    },
                    "first class": {
                        "nonstop": false,
                        "number": 17,
                        "price": 129.5
                    },
                    "second class": {
                        "nonstop": false,
                        "number": 200,
                        "price": 74.5
                    }
                }
            }
        ]
    },
    "id": "748b74b9dc7bb96a828b72cdd0b8eb492a608eb6",
    "type": "route"
}
```

## startRoute

### HTTP Request

```
GET /routes/d0bda205ca3042fd5398c7e2c8bc163803080888/start
```

### Response body
```json
{
    "data": {
        "succMsg": "Start Route succeeded!"
    },
    "type": "success"
}
```

## stopRoute

### HTTP Request

```
GET /routes/d0bda205ca3042fd5398c7e2c8bc163803080888/stop
```

### Response body
```json
{
    "data": {
        "succMsg": "Stop Route succeeded!"
    },
    "type": "success"
}
```

## queryTickets

### HTTP Request
```
POST /routes/:routeId/tickets
```

### Request body
```json
{
    "l": 0,
    "r": 1
}
```

### Response
```json
{
    "tickets": {
        "bussiness class": {
            "price": 239.5,
            "number": 5,
            "nonstop": false
        },
        "second class": {
            "price": 74.5,
            "number": 200,
            "nonstop": false
        },
        "first class": {
            "price": 129.5,
            "number": 17,
            "nonstop": false
        }
    },
    "endStation": {
        "arriveTime": "2017/5/5 13:15",
        "distance": 2,
        "stationId": "a2ee7702ff88ee1d147e5220bf767d91e3a327fc",
        "flags": 2
    },
    "startStation": {
        "distance": 1,
        "stationId": "c82516e6c86b81590b7e397aae8fb8d217402eab",
        "leaveTime": "2017/5/5 12:23",
        "flags": 1
    }
}
```

## bookTickets

### HTTP Request
```
PUT /routes/:routeId/tickets
```

### Request body
```json
{
    "l": 0,
    "r": 1,
    "ticketNumber": 2,
    "userId": "756b418e1d5b6aaf482f9db33112a20fb0350fe1",
    "ticketType": "first class"
}
```

### Response body

- `ticketPrice` is total price.
- The order is immediately attached to the user.

```json
{
    "data": {
        "ticketPrice": 259.0,
        "startStationId": "c4d36e6e39c1bee38644761c8d562fd5c5ede28b",
        "ticketNumber": 2,
        "routeId": "f5f2225f7758cc97253c658c42320f40b1db282a",
        "endStationId": "089c3301cde7f387d4d53c0a0be7bfe51ad32229",
        "ticketType": "first class"
    },
    "type": "order",
    "id": "c5e32ab22f6227e9f177612148f044c72624fa1f"
}
```
