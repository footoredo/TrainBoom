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
    "data": {
        "routes": [
            "169993781d665438d3a3cf267187cc07a93c75e6",
            "d0bda205ca3042fd5398c7e2c8bc163803080888"
        ]
    },
    "type": "routesList"
}
```

## get

### HTTP Request

```
GET /routes/:routeId
```

### Response body
```
GET /routes/d0bda205ca3042fd5398c7e2c8bc163803080888
```
```json
{
    "data": {
        "informations": [
            {
                "data": {
                    "distance": 1,
                    "flags": 1,
                    "leaveTime": "2017/5/5 12:23",
                    "stationId": "b8150fef4be4a0f7b8dcffbeeabc68ab48ca616b"
                },
                "id": "9d56bd74e1aecb696102fe5046bbf02e34eebe2c",
                "type": "information"
            },
            {
                "data": {
                    "arriveTime": "2017/5/5 13:15",
                    "distance": 2,
                    "flags": 2,
                    "stationId": "5e77d71fabefb3f965739abb9b962c2d1e0d048f"
                },
                "id": "f9a121d4a11733ba44b2a5f8d8111ce3b0410bc0",
                "type": "information"
            }
        ],
        "n": 2,
        "name": "G105",
        "segments": [
            {
                "data": {
                    "tickets": {
                        "bussiness class": {
                            "data": {
                                "number": 5,
                                "price": 239.5
                            },
                            "type": "attribute"
                        },
                        "first class": {
                            "data": {
                                "number": 15,
                                "price": 129.5
                            },
                            "type": "attribute"
                        },
                        "second class": {
                            "data": {
                                "number": 200,
                                "price": 74.5
                            },
                            "type": "attribute"
                        }
                    }
                },
                "id": "9c8c22b9eaefe55fb675841060eb9dc1ee1d41c1",
                "type": "segment"
            }
        ]
    },
    "id": "d0bda205ca3042fd5398c7e2c8bc163803080888",
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
GET /routes/:routeId/tickets
```

### Request body
```json
{
    "startStation": "b8150fef4be4a0f7b8dcffbeeabc68ab48ca616b",
    "endStation": "5e77d71fabefb3f965739abb9b962c2d1e0d048f"
}
```

### Response
```json
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
    "id": "5d0f1e873f3c2da2e877cb9dbcbbee4d6ce669b5"
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
    "startStationId": "b8150fef4be4a0f7b8dcffbeeabc68ab48ca616b",
    "ticketNumber": 2,
    "userId": "756b418e1d5b6aaf482f9db33112a20fb0350fe1",
    "ticketType": "first class",
    "endStationId": "5e77d71fabefb3f965739abb9b962c2d1e0d048f"
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
