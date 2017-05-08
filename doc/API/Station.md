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
    "id": "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
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
    "data": {
        "stations": [
            "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
            "da81e4553f89751731dea2adb486a4582ba3fa1d"
        ]
    },
    "type": "stationsList"
}
```

## get

### HTTP Request

```
GET /stations/:stationId
```

### Response body
```
GET /stations/45cd9b0bcc316846ddc27ffe18f2a68a32a17ada
```
```json
{
    "data": {
        "name": "Changzhou North"
    },
    "id": "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
    "type": "station"
}
```

## update

### HTTP Request

```
PUT /stations/:stationId
```

The rest is same as `insert`, except for all informations are optional.

## queryRoutes

### HTTP Request

```
GET /stations/:stationId/routes
```

### Request body

```
POST /stations/45cd9b0bcc316846ddc27ffe18f2a68a32a17ada/routes
```

```json
{
    "stationId": "da81e4553f89751731dea2adb486a4582ba3fa1d",
    "data": "2017/5/5"
}
```

### Response body

```json
{
    "data": {
        "routes": [
            "4bba9447b9f34656760f252e5695c9ae8974060a",
            "e762d6a5e17f3d696b9598876659428579ac828e"
        ]
    },
    "type": "routesList"
}
```
