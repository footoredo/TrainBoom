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
    "data": {
        "routeIntervals": [
            {
                "data": {
                    "l": 0,
                    "r": 1,
                    "routeId": "65eef8b3eda80c29b72a86420fab5de8d20c1388"
                },
                "type": "routeInterval"
            }
        ]
    },
    "type": "routeIntervalsList"
}
```
