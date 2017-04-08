# `CSV`

## Introduction

CSV=Comma-Separated Values，一种逗号分隔的文本格式。这个CSV Parser可以读取CSV文件并将封装成一个类，可以随机访问其中的元素。 

## Usage


```C++
CSV a;
```

## Member functions

Name|Description
--|--
[`(constructor)`]|构造函数
[`(destructor)`]|析构函数
[`operator=`]|赋值函数
[`size`]|返回总行数 
[`size(int x)`]|返回第x行元素个数(1 base) 
[`data(int x, int y)`]|返回第x行第y个元素(1 base) 
[`load(std::string filename)`]|从文件路径filename中读取CSV 

