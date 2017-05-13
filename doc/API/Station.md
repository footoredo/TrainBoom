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
    "stations": [
        "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada",
        "da81e4553f89751731dea2adb486a4582ba3fa1d"
    ]
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

## delete

### HTTP Request

```
DELETE /stations/:stationId
```

### Response body
```json
{
    "data": {
        "succMsg": "Delete station succeeded!"
    },
    "type": "success"
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
    "name": "Changzhou North"
}
```

### Response body
```json
{
   "stationId": "45cd9b0bcc316846ddc27ffe18f2a68a32a17ada"
}
```

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
    "stationName": "Shanghai-Hong-Qiao",
}
```

### Response body

```json
{
    "routes": [
        {
            "data": {
                "r": 1,
                "l": 0,
                "routeId": "e0a410fb61c71e3b08532f5e017ab91f3c3dc727"
            },
            "type": "routeInterval"
        }
    ]
}
```
