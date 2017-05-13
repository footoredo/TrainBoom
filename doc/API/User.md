# User

## insert

### HTTP Request

```
POST /users
```

### Request body

- `username`, `password` and `salt` are required.
- `gender`: 0 - male, 1 - femail, 2 - other
- `password` = sha1sum(`":rawpassword`|`:salt"`)

```json
{
    "username": "footoredo",
    "password": "0d0cd0941c15564d313dc1e342d3ebdfa6f6c350",
    "salt": "iamasalt",

    "avatar": "avatar.png",
    "realname": "zzh",
    "phone": "110",
    "email": "footoredo@gmail.com",
    "motto": "An apple a day, keep doctors away!",
    "gender": 0,
    "isRoot": true
}
```

### Response body

```json
{
   "type":"user",
   "id":"756b418e1d5b6aaf482f9db33112a20fb0350fe1",
   "data":{
      "username":"footoredo",
      "salt":"iamasalt",
      "password":"0d0cd0941c15564d313dc1e342d3ebdfa6f6c350",
      "avatar":"avatar.png",
      "realname":"zzh",
      "phone":"110",
      "email":"footoredo@gmail.com",
      "motto":"An apple a day, keep doctors away!",
      "gender":0,
      "isRoot":true
   }
}
```

## list

### HTTP Request

```
GET /users
```

### Response body
```json
{
    "users": [
        "756b418e1d5b6aaf482f9db33112a20fb0350fe1"
    ]
}
```

## get

### HTTP Request

```
GET /users/:userId
```

### Response body
```
GET /users/756b418e1d5b6aaf482f9db33112a20fb0350fe1
```
```json
{
   "type":"user",
   "id":"756b418e1d5b6aaf482f9db33112a20fb0350fe1",
   "data":{
      "username":"footoredo",
      "salt":"iamasalt",
      "password":"0d0cd0941c15564d313dc1e342d3ebdfa6f6c350",
      "avatar":"avatar.png",
      "realname":"zzh",
      "phone":"110",
      "email":"footoredo@gmail.com",
      "motto":"An apple a day, keep doctors away!",
      "gender":0,
      "isRoot":true
   }
}
```

## update

### HTTP Request

```
PUT /users/:userId
```

The rest is same as `insert`, except for all informations are optional.

## delete

### HTTP Request

```
DELETE /users/:userId
```

### Response body
```json
{
    "data": {
        "succMsg": "Delete user succeeded!"
    },
    "type": "success"
}
```

## listOrders

### HTTP Request
```
GET /users/:userId/orders
```

### Response body
```json
{
    "orders": [
        "c10fd1d8e080c6f1022fa589f1d65c1a7ed556bc"
    ]
}
```

## getOrder

### HTTP Request
```
GET /users/:userId/orders/:orderId
```

### Response body
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

## username

### HTTP Reqeust
```
POST /users/username
```

### Request body
```json
{
    "username": "footoredo"
}
```

### Response body
```json
{
    "userId": "dba0283f1e9356d6fe4654ab36fa8fcb9d44b421"
}

```
