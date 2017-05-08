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
   "type":"usersList",
   "data":{
      "users":[
         "756b418e1d5b6aaf482f9db33112a20fb0350fe1"
      ]
   }
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

## listOrders

### HTTP Request
```
GET /users/:userId/orders
```

### Response body
```json
{
    "data": {
        "orders": [
            "c10fd1d8e080c6f1022fa589f1d65c1a7ed556bc"
        ]
    },
    "type": "ordersList"
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
   "type": "userId",
   "data": {
      "userId": "f3a6e5276e18609a6845321ab858708bdf75c2db"
   }
}
```
