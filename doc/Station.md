# `Station`

## Introduction

车站信息的类。

## Members

Name|Type|Description
--|--|--
`id`|`id_t`|
`name`|std::string|车站名称（上海/上海南/...）
`toStation`|`util::map<id_t, util::set<id_t>>`|邻接表结构，key 为 station 的 id，value 为 route 的 id

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`add(id_t stationId, id_t routeId)`|增加一条通过 routeId 到 stationId 的边
`del(id_t stationId, id_t routeId)`|删除一条通过 routeId 到 stationId 的边
