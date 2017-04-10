# `util::Datetime::Duration`

## Introduction

表示一段时间的类。

## Members

Name|Type|Description
--|--|--
`day`|`Date_t`|日期
`hour`|`Date_t`|小时
`minute`|`Date_t`|分钟

## Member functions

Name|Description
--|--
`(constructor)`|构造函数（支持从字符串构造）
`format`|格式化时间
`operator string()`|格式化后转化成字符串
`operator+(Duration)`|
`operator-(Duration)`|
`getDay`|
`getHour`|
`getMinute`|
