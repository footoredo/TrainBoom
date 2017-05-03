# API Reference

## Resource types

### User

Method|HTTP request|Description
--|--|--
`insert`|`POST /users`| Insert a single user.
`list`|`GET /users`| Get the user list.
`get`|`GET /users/$userId`| Get information of a single user.
`update`|`PUT /users/$userId`| Update information of a single user.

### Station

Method|HTTP request|Description
--|--|--
`insert`|`POST /stations`| Insert a single station.
`list`|`GET /stations`| Get the station list.
`get`|`GET /stations/$stationId`| Get information of a single station.
`update`|`PUT /stations/$stationId`| Update information of a single station.
`addRoute`|`POST /stations/$stationsId/routes`| Add a route to a station.
`delRoute`|`DELETE /stations/$stationsId/routes`| Delete a route of the station.
`getRoutes`|`GET /stations/$stationsId/routes`| Get the routes list of the station.

### Route

Method|HTTP request|Description
--|--|--
`insert`|`POST /routes`| Insert a single route.
`list`|`GET /routes`| Get the routes list.
`get`|`GET /routes/$routeId`| Get information of a single route.
`update`|`PUT /routes/$routeId`| Update information of a single route.
`delete`|`DELETE /routes/$routeId`| Delete a single route.
`queryTickets`|`GET /routes/$routeId/tickets`| Get ticket information of a route.
`bookTickets`|`POST /routes/$routeId/tickets`| Book some tickets.
