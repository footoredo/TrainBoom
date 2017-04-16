# `Station`

## Introduction

车站信息的类。

## Members

Name|Type|Description
--|--|--
`id`|`id_t`|
`name`|`std::string`|车站名称（上海/上海南/...）
`toStation`|`util::map<id_t, util::set<id_t>>`|邻接表结构，key 为 station 的 id，value 为 train 的 id

## Member functions

Name|Description
--|--
`Station(id, name)`|构造函数
`getId()`|
`getName()`|
`add(id_t stationId, id_t trainId)`|增加一条通过 trainId 到 stationId 的边
`del(id_t stationId, id_t trainId)`|删除一条通过 trainId 到 stationId 的边
`const util::set<id_t>& query(id_t stationId)`|返回到 stationId 所有车次的 set
