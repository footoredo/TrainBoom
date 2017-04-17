# `Train`

## Introduction

某一车次某一天的运行计划。

## Members

Name|Type|Description
--|--|--
`id` | `id_t` |
`name` | `std::string` | 车次编号
`route` | `Route` | 运行路线 (public)

## Friend classes
- `Route`

## Member functions

Name|Description
--|--
[`(constructor)`](Train/Train.md)|构造函数
`getId`|
`getName`|
`getRunningTime`|获取运行日期 (以发车时的日期为准)
