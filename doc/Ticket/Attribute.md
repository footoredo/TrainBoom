# `Ticket::Attibute`

## Introduction

车票信息的类。

## Members

Name|Type|Description
--|--|--
`price`|`double`|车票单价
`number`|`size_t`|车票余量

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`operator=`|赋值函数

## Friend functions

Name|Description
--|--
`Attribute Ticket::modifyNumber(Attribute, int)`|车票余量变化 (+/-)
`Attribute mergeAttribute(Attribute, Attribute)`|合并车票信息 (单价相加，余量取 min)
