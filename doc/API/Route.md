# Route

## insert

### HTTP Request

```
POST /routes
```

### Request body

- In `flags`, 1 stands for start station, 2 stands for end station, 0 stands for normal station.
- In `segments.tickets`, `number` is optional with a default value of 2000.

```json
{
    "informations": [
        {
            "distance": 1,
            "flags": 1,
            "leaveTime": "12:23",
            "stationName": "Changzhou-North"
        },
        {
            "arriveTime": "13:15",
            "distance": 2,
            "flags": 2,
            "stationName": "Shanghai-Hong-Qiao"
        }
    ],
    "n": 2,
    "name": "G106",
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
                    "number": 15,
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
GET /routes/779d46f7840e11bdd20f61d36d5d8ff9cd441909
```
```json
{
    "data": {
        "informations": [
            {
                "distance": 1,
                "flags": 1,
                "leaveTime": "12:23",
                "stationName": "Changzhou-North"
            },
            {
                "arriveTime": "13:15",
                "distance": 2,
                "flags": 2,
                "stationName": "Shanghai-Hong-Qiao"
            }
        ],
        "n": 2,
        "name": "G106",
        "running": true,
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
                        "number": 15,
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
    "id": "779d46f7840e11bdd20f61d36d5d8ff9cd441909",
    "type": "route"
}
```

## update

### HTTP Request

```
PUT /route/:routeId
```

The rest is same as `insert`, all informations are **required**.

## delete

### HTTP Request

```
DELETE /route/:routeId
```

### Response body
```json
{
    "data": {
        "succMsg": "Delete route succeeded!"
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
    "date": "2017/5/5",
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
        "arriveTime": "13:15",
        "distance": 2,
        "stationName": "Shanghai-Hong-Qiao",
        "flags": 2
    },
    "startStation": {
        "distance": 1,
        "stationName": "Changzhou-North",
        "leaveTime": "12:23",
        "flags": 1
    }
}
```

## bookTickets

### HTTP Request
```
POST /routes/:routeId/tickets/book
```

### Request body
```json
{
    "date": "2017/5/5",
    "l": 0,
    "r": 1,
    "ticketNumber": 2,
    "userId": "756b418e1d5b6aaf482f9db33112a20fb0350fe1",
    "ticketType": "first class"
}
```

### Response body

- `ticketPrice` is the total price.
- The order is immediately attached to the user.

```json
{
    "data": {
        "endStationName": "Shanghai-Hong-Qiao",
        "routeInterval": {
            "l": 0,
            "r": 1,
            "routeId": "c87297363a7a0b67e9be2cd6a16c62afe3646914",
            "routeName": "G106"
        },
        "startStationName": "Changzhou-North",
        "ticketNumber": 2,
        "ticketPrice": 259.0,
        "ticketType": "first class"
    },
    "id": "6bbf03d66511bff98746a98bf0f12a786df50d47",
    "type": "order"
}
```

## refundTickets

### HTTP Request
```
POST /routes/:routeId/tickets/refund
```

### Request body
```json
{
    "date": "2017/5/5",
    "l": 0,
    "r": 1,
    "ticketNumber": 2,
    "userId": "756b418e1d5b6aaf482f9db33112a20fb0350fe1",
    "ticketType": "first class"
}
```
