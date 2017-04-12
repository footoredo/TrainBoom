# `util::IntervalManip`

## Introduction

管理一段数组的类，支持区间查询和区间修改，不过不支持插入或删除元素。

## Template parameters

- `T`: 元素类型
- `M`: 修改元素类型
- `Modifier(T, M, size_t, size_t)`: 修改函数，最后两个参数为区间左右端点
- `MergerT(T, T)`: 合并答案的函数
- `MergerM(M, M)`: 合并修改的函数


## Member functions

Name|Description
--|--
[`(constructor)`](IntervalManip/IntervalManip.md)|构造函数
`rebuild`|重构函数
`query(size_t l, size_t r)`|
`modify(size_t l, size_t r, cosnt M& m)`|
