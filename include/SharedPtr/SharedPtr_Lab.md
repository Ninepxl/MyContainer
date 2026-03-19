# SharedPtr Lab - 共享所有权智能指针

## 概述

在这个 Lab 中，你将实现一个简化版的 `std::shared_ptr` —— 共享所有权智能指针。

与你之前写的 `UniquePtr` 不同，`SharedPtr` 允许多个指针指向同一个对象。它通过**引用计数 (Reference Counting)** 来追踪有多少个 `SharedPtr` 共享同一块内存，当最后一个 `SharedPtr` 被销毁时，才真正释放内存。

## 核心概念

### 引用计数 (Reference Counting)

想象一个房间里有一盏灯，每个进入房间的人会把计数器 +1，离开时 -1。当计数器归零时，最后一个人负责关灯。

```
SharedPtr<int> p1(new int(42));   // 引用计数 = 1，p1 进入房间
SharedPtr<int> p2 = p1;           // 引用计数 = 2，p2 也进来了
SharedPtr<int> p3 = p2;           // 引用计数 = 3，p3 也进来了

p1.reset();                       // 引用计数 = 2，p1 离开
p2.reset();                       // 引用计数 = 1，p2 离开
// p3 析构时                      // 引用计数 = 0，p3 关灯（delete）
```

### 为什么引用计数要放在堆上？

这是一个关键的设计问题。思考一下：

> 如果 `_ref_count` 是一个普通的 `size_t` 成员变量（而不是 `size_t*` 指针），
> 当你拷贝一个 SharedPtr 时，每个副本都有自己独立的 `_ref_count`。
> 修改一个副本的计数，其他副本看不到变化。

所以引用计数必须是一个**所有副本共享的堆上变量**，通过指针 `size_t*` 来访问。

```
p1._ref_count ──┐
p2._ref_count ──┼──> [ 3 ]  （堆上的同一个 size_t）
p3._ref_count ──┘
```

## 已提供的代码

在 `include/SharedPtr/SharedPtr.h` 中，我已经给你提供了：

- 两个私有成员变量：`_ptr` 和 `_ref_count`
- 默认构造函数和参数构造函数的实现
- 析构函数的签名（需要你实现）

## 你的任务

### Part 1: 析构函数

析构函数是 SharedPtr 的核心。请在头文件中完成 `~SharedPtr()` 的实现。

**思考题：**
1. 如果 `_ref_count` 是 `nullptr`（默认构造的空指针），析构时需要做什么？
2. 如果 `*_ref_count` 减 1 后变为 0，说明什么？应该做什么？
3. 如果 `*_ref_count` 减 1 后仍然大于 0，应该做什么？
4. 除了 `delete _ptr`，还需要 `delete` 什么？

### Part 2: 拷贝语义

这是 SharedPtr 和 UniquePtr 最大的区别 —— SharedPtr **允许拷贝**。

你需要声明并实现：

**拷贝构造函数** `SharedPtr(const SharedPtr& other)`

- 思考：拷贝时，`_ptr` 和 `_ref_count` 应该怎么处理？
- 思考：引用计数应该增加还是减少？增加多少？
- 边界情况：如果 `other` 是空的（`other._ptr == nullptr`），怎么办？

**拷贝赋值运算符** `SharedPtr& operator=(const SharedPtr& other)`

这个比拷贝构造复杂一些，因为 `*this` 可能已经管理着一个对象。

- 思考：赋值前，需要先对自己当前管理的资源做什么？（提示：类似析构的逻辑）
- 思考：需要检查自赋值吗？（`p = p` 的情况）
- 步骤建议：
  1. 检查自赋值
  2. 释放自己当前的资源（减少旧的引用计数，可能需要 delete）
  3. 拷贝 other 的 `_ptr` 和 `_ref_count`
  4. 增加新的引用计数
  5. 返回 `*this`

### Part 3: 移动语义

移动语义的目的是**转移所有权而不改变引用计数**。

你需要声明并实现：

**移动构造函数** `SharedPtr(SharedPtr&& other) noexcept`

- 思考：移动和拷贝的区别是什么？移动时引用计数需要变化吗？
- 提示：移动就是"偷"，把 other 的 `_ptr` 和 `_ref_count` 都拿过来，然后把 other 置空

**移动赋值运算符** `SharedPtr& operator=(SharedPtr&& other) noexcept`

- 思考：和拷贝赋值类似，但转移所有权时不需要修改引用计数
- 别忘了先释放自己当前的资源

### Part 4: 指针操作

这些和 UniquePtr 基本一样，你应该已经很熟悉了：

| 函数 | 说明 |
|------|------|
| `T& operator*() const` | 解引用，返回对象引用 |
| `T* operator->() const` | 箭头运算符，返回裸指针 |
| `T* get() const` | 获取裸指针，不转移所有权 |

### Part 5: 辅助函数

**`size_t use_count() const`**

- 返回当前的引用计数
- 如果 `_ref_count` 为 `nullptr`，应该返回什么？

**`void reset(T* ptr = nullptr)`**

- 释放当前管理的资源（减少引用计数，可能 delete）
- 如果 `ptr` 不为空，接管新指针并创建新的引用计数
- 如果 `ptr` 为空，变成空的 SharedPtr
- 提示：reset 的前半部分逻辑和析构函数很像，考虑能否复用

**`explicit operator bool() const`**

- 和 UniquePtr 一样，判断是否持有有效指针

### Part 6: make_shared（选做进阶）

```cpp
template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args);
```

和 `make_unique` 类似，用完美转发在堆上构造对象。

> 注意：真正的 `std::make_shared` 会把对象和引用计数分配在同一块内存中以提高性能，
> 我们的简化版不需要这么做，直接 `new T(...)` 即可。

## 内存模型图

理解下面这张图对你实现 SharedPtr 非常重要：

```
  栈 (Stack)                          堆 (Heap)
  ┌──────────────┐
  │ p1           │
  │  _ptr ───────┼──────────────────> [ int: 42 ]
  │  _ref_count ─┼──────────┐
  └──────────────┘          │
                            ├──────> [ size_t: 2 ]
  ┌──────────────┐          │
  │ p2           │          │
  │  _ptr ───────┼──────────┼──────> [ int: 42 ]  (同一个对象)
  │  _ref_count ─┼──────────┘
  └──────────────┘

  p1 和 p2 的 _ptr 指向同一个 int 对象
  p1 和 p2 的 _ref_count 指向同一个计数器
```

## 常见错误提醒

1. **忘记 delete `_ref_count`**：当引用计数归零时，不仅要 `delete _ptr`，还要 `delete _ref_count`
2. **拷贝赋值时忘记释放旧资源**：先减少旧的引用计数，再接管新的
3. **移动后忘记置空 other**：移动后 `other._ptr` 和 `other._ref_count` 都要设为 `nullptr`
4. **空指针的引用计数**：默认构造的 SharedPtr，`_ref_count` 是 `nullptr`，访问前要检查

## 编译和测试

```bash
cd build && cmake .. && make SharedPtrTest && ./SharedPtrTest
```

测试文件在 `test/SharedPtrTest.cc`，共约 20 个测试用例。
建议你按 Part 的顺序实现，每完成一个 Part 就运行测试看看通过了几个。

测试标签对应关系：
- `[Construct]` → Part 1
- `[Copy]` → Part 2
- `[Move]` → Part 3
- `[Access]` → Part 4
- `[UseCount]` `[Reset]` `[Bool]` → Part 5
- `[MakeShared]` → Part 6
- `[Integration]` → 综合场景

你可以只运行某个 Part 的测试：
```bash
./SharedPtrTest "[Copy]"
```

## 参考资料

- [cppreference - std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)
- [learncpp - std::shared_ptr](https://www.learncpp.com/cpp-tutorial/stdshared_ptr/)
- [Back to Basics: Smart Pointers - CppCon 2019 (YouTube)](https://www.youtube.com/watch?v=xGDLkt-jBJ4)
- [Reference Counting 原理 (Wikipedia)](https://en.wikipedia.org/wiki/Reference_counting)
