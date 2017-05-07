# Station

## insert

### HTTP Request

```
POST /stations
```

### Request body

```json
{
    "name": "Changzhou North"
}
```

### Response body

```json
{
    "data": {
        "name": "Changzhou North"
    },
    "id": "96a13a96e44750b09b85380bfe163d75dd945a11",
    "type": "station"
}
```

## list

### HTTP Request

```
GET /stations
```

### Response body
```json
{
   "type":"stationsList",
   "data":{
      "stations":[
         "756b418e1d5b6aaf482f9db33112a20fb0350fe1"
      ]
   }
}
```

## get

### HTTP Request

```
GET /stations/:stationId
```

### Response body
```
GET /stations/756b418e1d5b6aaf482f9db33112a20fb0350fe1
```
```json
{
   "type":"station",
   "id":"756b418e1d5b6aaf482f9db33112a20fb0350fe1",
   "data":{
      "stationname":"footoredo",
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
PUT /stations/:stationId
```

The rest is same as `insert`, except for all informations are optional.
