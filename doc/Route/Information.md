# `Route::Information`

## Introduction

单一站点信息的类（不包括车票信息）

## Members

Name|Type|Description
--|--|--
`flags`|`size_t`|是否为起、终点站的 flag
`stationId`|`id_t`|站点 id
`distance`|`size_t`|到此处的累计里程 (km)
`arriveTime`|`util::Datetime::Datetime`|到站时间（起点站无法 access）
`leaveTime`|`util::Datetime::Datetime`|离开时间（终点站无法 access）

## Member classes

Name|Description
--|--
[`Information`](Route/Information.md)|站点信息

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
