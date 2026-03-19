/*
 * UniquePtr 单元测试
 * 使用 Catch 测试框架
 *
 * 运行方式: 在 main.cc 中将 if(true) 改为 if(false)，
 *           并将 argv 中的 tag 改为 "[UniquePtr]"
 *
 * 测试覆盖:
 *   1. 构造与析构
 *   2. 禁止拷贝（编译期检查，见最后的静态断言）
 *   3. 移动语义
 *   4. 指针操作 (*, ->, get)
 *   5. release / reset
 *   6. 布尔转换
 *   7. make_unique（选做）
 */

#include "UniquePtr/UniquePtr.h"
#include "catch.hpp"
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// ==================== 辅助结构体 ====================

// 用于追踪构造和析构次数，验证内存是否正确释放
struct Tracker {
    static int alive;  // 当前存活的 Tracker 对象数量
    int value;

    Tracker() : value(0) { ++alive; }
    Tracker(int v) : value(v) { ++alive; }
    ~Tracker() { --alive; }
};
int Tracker::alive = 0;

// 用于测试 operator-> 的结构体
struct Point {
    int x;
    int y;
    int sum() const { return x + y; }
};

// ==================== 1. 构造与析构 ====================

TEST_CASE("UniquePtr - 默认构造函数创建空指针", "[UniquePtr][Construct]") {
    MySTL::UniquePtr<int> p;
    REQUIRE(p.get() == nullptr);
}

TEST_CASE("UniquePtr - 参数构造函数接管裸指针", "[UniquePtr][Construct]") {
    int* raw = new int(42);
    MySTL::UniquePtr<int> p(raw);
    REQUIRE(p.get() == raw);
    REQUIRE(*p == 42);
}

TEST_CASE("UniquePtr - 析构函数释放内存", "[UniquePtr][Construct]") {
    Tracker::alive = 0;
    {
        MySTL::UniquePtr<Tracker> p(new Tracker(10));
        REQUIRE(Tracker::alive == 1);
    }
    // 离开作用域后，Tracker 应该被析构
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("UniquePtr - 管理 nullptr 析构不崩溃", "[UniquePtr][Construct]") {
    MySTL::UniquePtr<int> p;
    // 离开作用域时析构 nullptr，不应崩溃
    REQUIRE(p.get() == nullptr);
}

// ==================== 2. 禁止拷贝（编译期静态检查） ====================

TEST_CASE("UniquePtr - 不可拷贝构造", "[UniquePtr][Copy]") {
    // 这个测试用静态断言验证：UniquePtr 不能被拷贝
    // 如果你的实现正确 delete 了拷贝构造函数，这里会通过
    STATIC_REQUIRE_FALSE(std::is_copy_constructible<MySTL::UniquePtr<int>>::value);
}

TEST_CASE("UniquePtr - 不可拷贝赋值", "[UniquePtr][Copy]") {
    STATIC_REQUIRE_FALSE(std::is_copy_assignable<MySTL::UniquePtr<int>>::value);
}

// ==================== 3. 移动语义 ====================

TEST_CASE("UniquePtr - 移动构造转移所有权", "[UniquePtr][Move]") {
    MySTL::UniquePtr<int> p1(new int(99));
    int* raw = p1.get();

    MySTL::UniquePtr<int> p2(std::move(p1));

    // p2 接管了指针
    REQUIRE(p2.get() == raw);
    REQUIRE(*p2 == 99);
    // p1 变为空
    REQUIRE(p1.get() == nullptr);
}

TEST_CASE("UniquePtr - 移动赋值转移所有权", "[UniquePtr][Move]") {
    MySTL::UniquePtr<int> p1(new int(100));
    MySTL::UniquePtr<int> p2(new int(200));
    int* raw1 = p1.get();

    p2 = std::move(p1);

    // p2 现在指向原来 p1 的对象
    REQUIRE(p2.get() == raw1);
    REQUIRE(*p2 == 100);
    // p1 变为空
    REQUIRE(p1.get() == nullptr);
}

TEST_CASE("UniquePtr - 移动赋值释放旧资源", "[UniquePtr][Move]") {
    Tracker::alive = 0;
    MySTL::UniquePtr<Tracker> p1(new Tracker(1));
    MySTL::UniquePtr<Tracker> p2(new Tracker(2));
    REQUIRE(Tracker::alive == 2);

    p2 = std::move(p1);

    // p2 原来管理的 Tracker(2) 应该被释放
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p2->value == 1);
    REQUIRE(p1.get() == nullptr);
}

TEST_CASE("UniquePtr - 移动构造后析构不会 double free", "[UniquePtr][Move]") {
    Tracker::alive = 0;
    {
        MySTL::UniquePtr<Tracker> p1(new Tracker(42));
        MySTL::UniquePtr<Tracker> p2(std::move(p1));
        REQUIRE(Tracker::alive == 1);
    }
    // 两个 UniquePtr 都析构了，但只 delete 一次
    REQUIRE(Tracker::alive == 0);
}

// ==================== 4. 解引用与箭头运算符 ====================

TEST_CASE("UniquePtr - operator* 解引用", "[UniquePtr][Access]") {
    MySTL::UniquePtr<int> p(new int(7));
    REQUIRE(*p == 7);

    // 通过解引用修改值
    *p = 14;
    REQUIRE(*p == 14);
}

TEST_CASE("UniquePtr - operator-> 访问成员", "[UniquePtr][Access]") {
    MySTL::UniquePtr<Point> p(new Point{3, 4});
    REQUIRE(p->x == 3);
    REQUIRE(p->y == 4);
    REQUIRE(p->sum() == 7);
}

TEST_CASE("UniquePtr - get() 返回裸指针但不转移所有权", "[UniquePtr][Access]") {
    MySTL::UniquePtr<int> p(new int(55));
    int* raw = p.get();

    REQUIRE(raw != nullptr);
    REQUIRE(*raw == 55);
    // get() 后 UniquePtr 仍然持有指针
    REQUIRE(p.get() == raw);
}

// ==================== 5. release 和 reset ====================

TEST_CASE("UniquePtr - release 释放所有权", "[UniquePtr][Release]") {
    MySTL::UniquePtr<int> p(new int(88));
    int* raw = p.release();

    // release 后 UniquePtr 变为空
    REQUIRE(p.get() == nullptr);
    // 返回的裸指针仍然有效
    REQUIRE(*raw == 88);

    // 调用者负责释放
    delete raw;
}

TEST_CASE("UniquePtr - release 空指针返回 nullptr", "[UniquePtr][Release]") {
    MySTL::UniquePtr<int> p;
    int* raw = p.release();
    REQUIRE(raw == nullptr);
}

TEST_CASE("UniquePtr - reset 无参数释放并置空", "[UniquePtr][Reset]") {
    Tracker::alive = 0;
    MySTL::UniquePtr<Tracker> p(new Tracker(5));
    REQUIRE(Tracker::alive == 1);

    p.reset();

    REQUIRE(p.get() == nullptr);
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("UniquePtr - reset 带参数替换指针", "[UniquePtr][Reset]") {
    Tracker::alive = 0;
    MySTL::UniquePtr<Tracker> p(new Tracker(1));
    REQUIRE(Tracker::alive == 1);

    p.reset(new Tracker(2));

    // 旧的被释放，新的被接管
    REQUIRE(Tracker::alive == 1);
    REQUIRE(p->value == 2);
}

// ==================== 6. 布尔转换 ====================

TEST_CASE("UniquePtr - 非空指针转换为 true", "[UniquePtr][Bool]") {
    MySTL::UniquePtr<int> p(new int(1));
    REQUIRE(static_cast<bool>(p) == true);

    // 在 if 语句中使用
    bool entered = false;
    if (p) {
        entered = true;
    }
    REQUIRE(entered);
}

TEST_CASE("UniquePtr - 空指针转换为 false", "[UniquePtr][Bool]") {
    MySTL::UniquePtr<int> p;
    REQUIRE(static_cast<bool>(p) == false);

    bool entered = false;
    if (p) {
        entered = true;
    }
    REQUIRE_FALSE(entered);
}

// ==================== 7. 综合场景 ====================

TEST_CASE("UniquePtr - 管理 string 类型", "[UniquePtr][Integration]") {
    MySTL::UniquePtr<std::string> p(new std::string("hello"));
    REQUIRE(*p == "hello");
    REQUIRE(p->size() == 5);

    *p += " world";
    REQUIRE(*p == "hello world");
}

TEST_CASE("UniquePtr - 在 vector 中存储 UniquePtr", "[UniquePtr][Integration]") {
    Tracker::alive = 0;
    {
        std::vector<MySTL::UniquePtr<Tracker>> vec;
        vec.push_back(MySTL::UniquePtr<Tracker>(new Tracker(1)));
        vec.push_back(MySTL::UniquePtr<Tracker>(new Tracker(2)));
        vec.push_back(MySTL::UniquePtr<Tracker>(new Tracker(3)));

        REQUIRE(Tracker::alive == 3);
        REQUIRE(vec[0]->value == 1);
        REQUIRE(vec[1]->value == 2);
        REQUIRE(vec[2]->value == 3);
    }
    // vector 析构时，所有 UniquePtr 都应该释放内存
    REQUIRE(Tracker::alive == 0);
}

// ==================== 8. make_unique（选做） ====================

TEST_CASE("UniquePtr - make_unique 创建对象", "[UniquePtr][MakeUnique]") {
    auto p = MySTL::make_unique<int>(42);
    REQUIRE(*p == 42);
}

TEST_CASE("UniquePtr - make_unique 创建自定义类型", "[UniquePtr][MakeUnique]") {
    auto p = MySTL::make_unique<Point>(Point{10, 20});
    REQUIRE(p->x == 10);
    REQUIRE(p->y == 20);
}
