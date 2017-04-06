# `Segment`

## Introduction

每一小段路径的车票信息。

## Members

Name|Type|Description
--|--|--
`startStation`|`Station`|起点站
`endStation`|`Station`|终点站
`startTime`|`util::Datetime`|到起点站时间
`stayTime`|`util::Datetime`|起点站停留时间（始发站为 0）
`endTime`|`util::Datetime`|到达终点站时间
`tickets`|`util::vector<Ticket>`|车票信息
`distance`|`size_t`|距离

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`operator=`|赋值函数
