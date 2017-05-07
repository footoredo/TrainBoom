# User

## insert

### HTTP Request

```
POST /users
```

### Request body

- `username`, `password` and `salt` are required.
- `gender`: 0 - male, 1 - femail, 2 - other

```json
{
    "username": "footoredo",
    "password": "password",
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
