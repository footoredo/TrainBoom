# API Reference

## Resource types

### [User](User.md)

Method|HTTP request|Description
--|--|--
`insert`|`POST /users`| Insert a single user.
`list`|`GET /users`| Get the user list.
`get`|`GET /users/:userId`| Get information of a single user.
`update`|`PUT /users/:userId`| Update information of a single user.
`delete`|`DELETE /users/:userId`| Delete a single user.
`username`|`POST /users/username`| Get userId by username.

### Station

Method|HTTP request|Description
--|--|--
`insert`|`POST /stations`| Insert a single station.
`list`|`GET /stations`| Get the station list.
`get`|`GET /stations/:stationId`| Get information of a single station.
`name`|`POST /stations/name`| Get stationId by its name.
`update`|`PUT /stations/:stationId`| Update information of a single station.
`delete`|`DELETE /stations/:stationId`| Delete a single user.
`queryRoutes`|`POST /stations/:stationId/routes`| Get the routes list of the station.

### Route

Method|HTTP request|Description
--|--|--
`insert`|`POST /routes`| Insert a single route.
`list`|`GET /routes`| Get the routes list.
`get`|`GET /routes/:routeId`| Get information of a single route.
`update`|`PUT /routes/:routeId`| Update information of a single route.
`delete`|`DELETE /routes/:routeId`| Delete a single route.
`queryTickets`|`POST /routes/:routeId/tickets`| Get ticket information of a route.
`bookTickets`|`PUT /routes/:routeId/tickets`| Book some tickets.

### RouteInterval

Method|HTTP request|Description
--|--|--
`get`|`POST /routeInterval/get`| Query via station name.

## Error Response

```json
{
    "type": "error",
    "data": {
        "errMsg": "xxxx"
    }
}
```
