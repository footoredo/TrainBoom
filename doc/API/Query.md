# Query

## queryRoute

### HTTP Request

```
POST /queryRoute
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
        "list": [
            {
                "data": {
                    "routeId": "fa2ff3f5e8a8f139b435f91926dd616b3f3172e5",
                    "tickets": {
                        "data": {
                            "tickets": {
                                "bussiness class": {
                                    "data": {
                                        "nonstop": false,
                                        "number": 5,
                                        "price": 239.5
                                    },
                                    "type": "attribute"
                                },
                                "first class": {
                                    "data": {
                                        "nonstop": false,
                                        "number": 15,
                                        "price": 129.5
                                    },
                                    "type": "attribute"
                                },
                                "second class": {
                                    "data": {
                                        "nonstop": false,
                                        "number": 200,
                                        "price": 74.5
                                    },
                                    "type": "attribute"
                                }
                            }
                        },
                        "id": "5d92c2c70ddab309e6c21a8fc582f35efd831e86",
                        "type": "segment"
                    }
                }
            }
        ]
    },
    "type": "queryResult"
}
```
