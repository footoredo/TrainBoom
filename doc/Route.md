# `Route`

## Introduction

运行路线的类。

## Members

Name|Type|Description
--|--|--
`segments`|`IntervalManip<Segment>`|每一小段路径车票信息（支持区间查询修改）
`isExpired`|`bool`|是否已过期

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`operator=`|赋值函数
[`sell`](Route/sell.md)|出售车票（连续一段）
`refund`|退订车票
