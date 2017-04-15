# `Train`

## Introduction

某一车次某一天的运行计划。

## Members

Name|Type|Description
--|--|--
`id` | `id_t` |
`name` | `std::string` | 车次编号
`running_time` | `util::Datetime::Datetime` | 运行日期 (以发车时的日期为准)
`route` | `Route` | 运行路线(public)

## Member functions

Name|Description
--|--
[`(constructor)`](Train/Train.md)|构造函数
[`(destructor)`](Train/~Train.md)|析构函数
[`operator=`](Train/operator=.md)|赋值函数
