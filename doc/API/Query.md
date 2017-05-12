# routeInterval

## get

### HTTP Request

```
POST /routeInterval/get
```

### Request body

```json
{
    "startStation": "Changzhou North",
    "endStation": "Shanghai Hong Qiao",
    "date": "2017/5/5"
}
```

### Response body

```json
{
    "routeIntervals": [
        {
            "data": {
                "l": 0,
                "r": 1,
                "routeId": "779d46f7840e11bdd20f61d36d5d8ff9cd441909",
                "routeName": "G106"
            },
            "id": "e5733b1f865e3a083dd05cd46b68492736b05d59",
            "type": "routeInterval"
        }
    ]
}
```
