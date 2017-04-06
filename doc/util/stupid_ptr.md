# `util::stupid_ptr`

## Introduction

就是 shared_ptr，对于每一个 object 有一个 counter 表示它被指向的次数，当 counter 降为 0 的时候会自动释放掉这块内存。

## Usage

单个变量：

```C++
stupid_ptr<T> p(new T);
stupid_ptr<T> = stupid_ptr<T>(new T);
```

数组：

```C++
stupid_ptr<T, array_deleter<T>> pa(new T[N]);
stupid_ptr<T, array_deleter<T>> = stupid_ptr<T, array_deleter<T>>(new T[N]);
```

其他使用与普通指针相同，支持`*`解引用与`->`操作，支持对 bool 的隐式类型转换，支持比较（只比较 object 是否是同一个），支持 swap。

- 不要与普通指针混用
- 不要手动析构或释放

## Template parameters


- `T`: 指针所指向的 object 的类型。注意当所指向内容为数组时仍用单个变量的类型，而不是`T*`。
- `Deleter`: 释放该 object 所用的指令。默认为[`single_deleter<T>`](stupid_ptr/single_deleter)，也提供了用于数组析构的[`array_deleter<T>`](stupid_ptr/array_deleter)。


## Member functions

Name|Description
--|--
[`(constructor)`](stupid_ptr/stupid_ptr.md)|构造函数
[`(destructor)`](stupid_ptr/~stupid_ptr.md)|析构函数
[`operator=`](stupid_ptr/operator=.md)|赋值函数
`operator*`|
`operator->`|
`operator bool`|判断是否为 null
`swap`|交换两个`stupid_ptr`
