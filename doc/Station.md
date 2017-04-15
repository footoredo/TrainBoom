# `Station`

## Introduction

车站信息的类。

## Members

Name|Type|Description
--|--|--
`id`|`id_t`|
`name`|std::string|车站名称（上海/上海南/...）
`toStation`|`util::map<size_t, util::stupid_ptr<Route>>`|邻接表结构

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`operator=`|赋值函数
