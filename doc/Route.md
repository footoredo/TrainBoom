# `Route`

## Introduction

运行路线的类。

## Members

Name|Type|Description
--|--|--
`id`|`id_t`|
`n`|`size_t`|站点数量
`stationsMap`|`util::map<size_t, size_t>`|站点id与其在此路线中的序号的对应关系
`stations`|`util::stupid_array<id_t>`|站点，存站点id
`distance`|`util::stupid_array<size_t>`|累计里程(km) (长度为n-1，不含起点站)
`arriveTime`|`util::stupid_array<util::Datetime::Datetime>`|到站时间（长度为n-1，不含起点站）
`leaveTime`|`util::stupid_array<util::Datetime::Datetime>`|离开时间（长度为n-1，不含终点站）
`segments`|`util::stupid_array<Segment>`|每一小段路径车票信息（支持区间查询修改，长度为n-1）
`segmentsIntervalManip`|`util::IntervalManip`|处理车票区间操作

## Member functions

Name|Description
--|--
`(constructor)`|构造函数
`(destructor)`|析构函数
`modifyTickets`|售出/退订车票（连续一段）
`queryTickets`|显示某一段路程的车票情况
