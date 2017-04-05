# `stupid_ptr`

## Introduction

就是 shared_ptr，对于每一个 object 有一个 counter 表示它被指向的次数，当 counter 降为 0 的时候会自动释放掉这块内存。

## Usage

单个变量：

    stupid_ptr<T> p(new T);
    // or stupid_ptr<T> = new T;

数组：

    stupid_ptr<T, array_deleter<T>> pa(new T[N]);
    // or stupid_ptr<T, array_deleter<T>> = new T[N];

其他使用与普通指针相同，支持`*`解引用与`->`操作，支持对 bool 的隐式类型转换，支持比较（只比较 object 是否是同一个），支持 swap。

- 不要与普通指针混用
- 不要手动析构或释放
