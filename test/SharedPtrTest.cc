/*
 * SharedPtr 单元测试
 * 使用 Catch 测试框架
 *
 * 运行方式:
 *   cd build && cmake .. && make SharedPtrTest && ./SharedPtrTest
 *
 * 运行特定标签:
 *   ./SharedPtrTest "[Copy]"
 *
 * 测试覆盖:
 *   Part 1: 构造与析构
 *   Part 2: 拷贝语义
 *   Part 3: 移动语义
 *   Part 4: 指针操作
 *   Part 5: use_count / reset / bool
 *   Part 6: make_shared（选做）
 *   综合场景
 */

#include "SharedPtr/SharedPtr.h"
#include "catch.hpp"
#include <string>
#include <utility>
#include <vector>

// ==================== 辅助结构体 ====================

struct Tracker {
    static int alive;
    int value;

    Tracker() : value(0) { ++alive; }
    Tracker(int v) : value(v) { ++alive; }
    ~Tracker() { --alive; }
};
int Tracker::alive = 0;

struct Point {
    int x;
    int y;
    int sum() const { return x + y; }
};

// ==================== Part 1: 构造与析构 ====================

TEST_CASE("SharedPtr - 默认构造函数创建空指针", "[SharedPtr][Construct]") {
    MySTL::SharedPtr<int> p;
    REQUIRE(p.get() == nullptr);
}

TEST_CASE("SharedPtr - 参数构造函数接管裸指针", "[SharedPtr][Construct]") {
    int* raw = new int(42);
    MySTL::SharedPtr<int> p(raw);
    REQUIRE(p.get() == raw);
    REQUIRE(*p == 42);
}

TEST_CASE("SharedPtr - 单个 SharedPtr 析构释放内存", "[SharedPtr][Construct]") {
    Tracker::alive = 0;
    {
        MySTL::SharedPtr<Tracker> p(new Tracker(10));
        REQUIRE(Tracker::alive == 1);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("SharedPtr - 空指针析构不崩溃", "[SharedPtr][Construct]") {
    MySTL::SharedPtr<int> p;
    REQUIRE(p.get() == nullptr);
    // 离开作用域不应崩溃
}

// ==================== Part 2: 拷贝语义 ====================

TEST_CASE("SharedPtr - 拷贝构造共享所有权", "[SharedPtr][Copy]") {
    MySTL::SharedPtr<int> p1(new int(99));
    MySTL::SharedPtr<int> p2(p1);

    // 两个指针指向同一个对象
    REQUIRE(p1.get() == p2.get());
    REQUIRE(*p1 == 99);
    REQUIRE(*p2 == 99);
    REQUIRE(p1.use_count() == 2);
    REQUIRE(p2.use_count() == 2);
}

TEST_CASE("SharedPtr - 拷贝构造后析构不会提前释放", "[SharedPtr][Copy]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p2;
    {
        MySTL::SharedPtr<Tracker> p1(new Tracker(5));
        p2 = p1;  // 拷贝赋值
        REQUIRE(Tracker::alive == 1);
        REQUIRE(p1.use_count() == 2);
    }
    // p1 析构了，但 p2 还在，对象不应被释放
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p2.use_count() == 1);
    REQUIRE(p2->value == 5);
}

TEST_CASE("SharedPtr - 拷贝赋值运算符", "[SharedPtr][Copy]") {
    MySTL::SharedPtr<int> p1(new int(10));
    MySTL::SharedPtr<int> p2(new int(20));

    p2 = p1;

    REQUIRE(p1.get() == p2.get());
    REQUIRE(*p2 == 10);
    REQUIRE(p1.use_count() == 2);
}

TEST_CASE("SharedPtr - 拷贝赋值释放旧资源", "[SharedPtr][Copy]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p1(new Tracker(1));
    MySTL::SharedPtr<Tracker> p2(new Tracker(2));
    REQUIRE(Tracker::alive == 2);

    p2 = p1;

    // Tracker(2) 应该被释放，因为没有其他 SharedPtr 指向它
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p2->value == 1);
}

TEST_CASE("SharedPtr - 拷贝空的 SharedPtr", "[SharedPtr][Copy]") {
    MySTL::SharedPtr<int> p1;
    MySTL::SharedPtr<int> p2(p1);

    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.get() == nullptr);
    REQUIRE(p1.use_count() == 0);
    REQUIRE(p2.use_count() == 0);
}

// ==================== Part 3: 移动语义 ====================

TEST_CASE("SharedPtr - 移动构造转移所有权", "[SharedPtr][Move]") {
    MySTL::SharedPtr<int> p1(new int(77));
    int* raw = p1.get();

    MySTL::SharedPtr<int> p2(std::move(p1));

    REQUIRE(p2.get() == raw);
    REQUIRE(*p2 == 77);
    REQUIRE(p2.use_count() == 1);
    // p1 被移动后应为空
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p1.use_count() == 0);
}

TEST_CASE("SharedPtr - 移动赋值转移所有权", "[SharedPtr][Move]") {
    MySTL::SharedPtr<int> p1(new int(100));
    MySTL::SharedPtr<int> p2(new int(200));
    int* raw1 = p1.get();

    p2 = std::move(p1);

    REQUIRE(p2.get() == raw1);
    REQUIRE(*p2 == 100);
    REQUIRE(p2.use_count() == 1);
    REQUIRE(p1.get() == nullptr);
}

TEST_CASE("SharedPtr - 移动赋值释放旧资源", "[SharedPtr][Move]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p1(new Tracker(1));
    MySTL::SharedPtr<Tracker> p2(new Tracker(2));
    REQUIRE(Tracker::alive == 2);

    p2 = std::move(p1);

    // Tracker(2) 被释放，Tracker(1) 转移给 p2
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p2->value == 1);
}

TEST_CASE("SharedPtr - 移动不改变引用计数", "[SharedPtr][Move]") {
    MySTL::SharedPtr<int> p1(new int(42));
    MySTL::SharedPtr<int> p2(p1);  // 拷贝，引用计数 = 2
    REQUIRE(p1.use_count() == 2);

    MySTL::SharedPtr<int> p3(std::move(p1));  // 移动 p1 给 p3

    // 引用计数仍然是 2（p2 和 p3），不是 3
    REQUIRE(p2.use_count() == 2);
    REQUIRE(p3.use_count() == 2);
    REQUIRE(p1.get() == nullptr);
}

// ==================== Part 4: 指针操作 ====================

TEST_CASE("SharedPtr - operator* 解引用", "[SharedPtr][Access]") {
    MySTL::SharedPtr<int> p(new int(7));
    REQUIRE(*p == 7);
    *p = 14;
    REQUIRE(*p == 14);
}

TEST_CASE("SharedPtr - operator-> 访问成员", "[SharedPtr][Access]") {
    MySTL::SharedPtr<Point> p(new Point{3, 4});
    REQUIRE(p->x == 3);
    REQUIRE(p->y == 4);
    REQUIRE(p->sum() == 7);
}

// ==================== Part 5: use_count / reset / bool ====================

TEST_CASE("SharedPtr - use_count 追踪引用计数", "[SharedPtr][UseCount]") {
    MySTL::SharedPtr<int> p1(new int(1));
    REQUIRE(p1.use_count() == 1);

    MySTL::SharedPtr<int> p2(p1);
    REQUIRE(p1.use_count() == 2);

    {
        MySTL::SharedPtr<int> p3(p1);
        REQUIRE(p1.use_count() == 3);
    }
    // p3 离开作用域
    REQUIRE(p1.use_count() == 2);
}

TEST_CASE("SharedPtr - reset 无参数", "[SharedPtr][Reset]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p(new Tracker(5));
    REQUIRE(Tracker::alive == 1);

    p.reset();

    REQUIRE(p.get() == nullptr);
    REQUIRE(p.use_count() == 0);
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("SharedPtr - reset 带参数替换指针", "[SharedPtr][Reset]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p(new Tracker(1));

    p.reset(new Tracker(2));

    REQUIRE(Tracker::alive == 1);
    REQUIRE(p->value == 2);
    REQUIRE(p.use_count() == 1);
}

TEST_CASE("SharedPtr - reset 不影响其他共享者", "[SharedPtr][Reset]") {
    Tracker::alive = 0;
    MySTL::SharedPtr<Tracker> p1(new Tracker(10));
    MySTL::SharedPtr<Tracker> p2(p1);
    REQUIRE(p1.use_count() == 2);

    p1.reset();

    // p1 退出共享，但 p2 仍然持有对象
    REQUIRE(p1.get() == nullptr);
    REQUIRE(p2.use_count() == 1);
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p2->value == 10);
}

TEST_CASE("SharedPtr - operator bool", "[SharedPtr][Bool]") {
    MySTL::SharedPtr<int> p1(new int(1));
    MySTL::SharedPtr<int> p2;

    REQUIRE(static_cast<bool>(p1) == true);
    REQUIRE(static_cast<bool>(p2) == false);

    if (p1) {
        REQUIRE(true);
    }
    if (p2) {
        REQUIRE(false); // 不应到达
    }
}

// ==================== 综合场景 ====================

TEST_CASE("SharedPtr - 管理 string 类型", "[SharedPtr][Integration]") {
    MySTL::SharedPtr<std::string> p1(new std::string("hello"));
    MySTL::SharedPtr<std::string> p2(p1);

    REQUIRE(*p1 == "hello");
    REQUIRE(*p2 == "hello");

    // 通过一个指针修改，另一个也能看到（因为是同一个对象）
    *p1 += " world";
    REQUIRE(*p2 == "hello world");
}

TEST_CASE("SharedPtr - 多次拷贝和析构的完整生命周期", "[SharedPtr][Integration]") {
    Tracker::alive = 0;
    {
        MySTL::SharedPtr<Tracker> p1(new Tracker(42));
        REQUIRE(Tracker::alive == 1);
        REQUIRE(p1.use_count() == 1);
        {
            MySTL::SharedPtr<Tracker> p2(p1);
            REQUIRE(p1.use_count() == 2);
            {
                MySTL::SharedPtr<Tracker> p3(p2);
                REQUIRE(p1.use_count() == 3);
            }
            REQUIRE(p1.use_count() == 2);
            REQUIRE(Tracker::alive == 1);
        }
        REQUIRE(p1.use_count() == 1);
        REQUIRE(Tracker::alive == 1);
    }
    REQUIRE(Tracker::alive == 0);
}

// ==================== Part 6: make_shared（选做） ====================

/*
 * 取消下面的注释来测试 make_shared
 * 确保你已经在 SharedPtr.h 中实现了 make_shared 函数
 */

// TEST_CASE("SharedPtr - make_shared 创建对象", "[SharedPtr][MakeShared]") {
//     auto p = MySTL::make_shared<int>(42);
//     REQUIRE(*p == 42);
//     REQUIRE(p.use_count() == 1);
// }

// TEST_CASE("SharedPtr - make_shared 创建自定义类型", "[SharedPtr][MakeShared]") {
//     auto p = MySTL::make_shared<Point>(Point{10, 20});
//     REQUIRE(p->x == 10);
//     REQUIRE(p->sum() == 30);
// }
