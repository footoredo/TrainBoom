# `Route`

## Introduction

运行路线的类。

## Members

Name|Type|Description
--|--|--
`n`|`size_t`|站点数量
`stationsMap`|`util::map<size_t, size_t>`|站点id与其在此路线中的序号的对应关系
`information`|`util::stupid_array<Information>`|各站点的信息
`segments`|`util::stupid_array<Segment>`|每一小段路径车票信息（支持区间查询修改，长度为n-1）
`segmentsIntervalManip`|`util::IntervalManip`|处理车票区间操作

## Member classes

Name|Description
--|--
[`Information`](Route/Information.md)|站点信息

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`modifyTickets`|售出/退订车票（连续一段）
`queryTickets`|显示某一段路程的车票情况
`Information& information(size_t)`|获取某个站点的站点信息
