#include "List/List.h"
#include "Vector/SimpleVector.h"
#include "catch.hpp"
#include <list>
#include <vector>

// ==================== 基础功能测试 ====================
using namespace MyList;

TEST_CASE("List - 空链表初始状态", "[List][Empty]") {
  List l;
  auto vec = l.print();
  REQUIRE(vec.empty());
}

TEST_CASE("List - 单元素插入和删除", "[List][Single]") {
  SECTION("push_back单元素") {
    List l;
    l.push_back(42);
    auto vec = l.print();
    REQUIRE(vec.size() == 1);
    REQUIRE(vec[0] == 42);
  }

  SECTION("push_front单元素") {
    List l;
    l.push_front(42);
    auto vec = l.print();
    REQUIRE(vec.size() == 1);
    REQUIRE(vec[0] == 42);
  }

  SECTION("push_back后pop_back") {
    List l;
    l.push_back(42);
    l.pop_back();
    auto vec = l.print();
    REQUIRE(vec.empty());
  }

  SECTION("push_front后pop_front") {
    List l;
    l.push_front(42);
    l.pop_front();
    auto vec = l.print();
    REQUIRE(vec.empty());
  }

  SECTION("push_back后pop_front") {
    List l;
    l.push_back(42);
    l.pop_front();
    auto vec = l.print();
    REQUIRE(vec.empty());
  }

  SECTION("push_front后pop_back") {
    List l;
    l.push_front(42);
    l.pop_back();
    auto vec = l.print();
    REQUIRE(vec.empty());
  }
}

// ==================== 多元素操作测试 ====================

TEST_CASE("List - push_back多元素", "[List][Insert]") {
  List l;
  std::list<int> stdL;

  std::vector<int> testData = {1, 2, 3, 4, 5};

  for (int val : testData) {
    l.push_back(val);
    stdL.push_back(val);
  }

  auto vec = l.print();
  REQUIRE(vec.size() == testData.size());

  int index = 0;
  for (auto &i : stdL) {
    REQUIRE(vec[index++] == i);
  }
}

TEST_CASE("List - push_front多元素", "[List][Insert]") {
  List l;
  std::list<int> stdL;

  std::vector<int> testData = {1, 2, 3, 4, 5};

  for (int val : testData) {
    l.push_front(val);
    stdL.push_front(val);
  }

  auto vec = l.print();
  REQUIRE(vec.size() == testData.size());

  int index = 0;
  for (auto &i : stdL) {
    REQUIRE(vec[index++] == i);
  }
}

TEST_CASE("List - 混合插入操作", "[List][Insert]") {
  List l;
  std::list<int> stdL;

  // 混合使用push_back和push_front
  l.push_back(1);
  stdL.push_back(1); // [1]
  l.push_front(2);
  stdL.push_front(2); // [2, 1]
  l.push_back(3);
  stdL.push_back(3); // [2, 1, 3]
  l.push_front(4);
  stdL.push_front(4); // [4, 2, 1, 3]
  l.push_back(5);
  stdL.push_back(5); // [4, 2, 1, 3, 5]

  auto vec = l.print();
  REQUIRE(vec.size() == 5);

  int index = 0;
  for (auto &i : stdL) {
    REQUIRE(vec[index++] == i);
  }
}

// ==================== 删除操作测试 ====================

TEST_CASE("List - pop_back多元素", "[List][Delete]") {
  List l;
  std::list<int> stdL;

  // 先插入多个元素
  for (int i = 1; i <= 5; i++) {
    l.push_back(i);
    stdL.push_back(i);
  }

  // 删除尾部元素
  SECTION("删除一个尾部元素") {
    l.pop_back();
    stdL.pop_back();

    auto vec = l.print();
    int index = 0;
    for (auto &i : stdL) {
      REQUIRE(vec[index++] == i);
    }
  }

  SECTION("删除多个尾部元素") {
    l.pop_back();
    l.pop_back();
    l.pop_back();
    stdL.pop_back();
    stdL.pop_back();
    stdL.pop_back();

    auto vec = l.print();
    int index = 0;
    for (auto &i : stdL) {
      REQUIRE(vec[index++] == i);
    }
  }
}

TEST_CASE("List - pop_front多元素", "[List][Delete]") {
  List l;
  std::list<int> stdL;

  // 先插入多个元素
  for (int i = 1; i <= 5; i++) {
    l.push_back(i);
    stdL.push_back(i);
  }

  SECTION("删除一个头部元素") {
    l.pop_front();
    stdL.pop_front();

    auto vec = l.print();
    int index = 0;
    for (auto &i : stdL) {
      REQUIRE(vec[index++] == i);
    }
  }

  SECTION("删除多个头部元素") {
    l.pop_front();
    l.pop_front();
    l.pop_front();
    stdL.pop_front();
    stdL.pop_front();
    stdL.pop_front();

    auto vec = l.print();
    int index = 0;
    for (auto &i : stdL) {
      REQUIRE(vec[index++] == i);
    }
  }
}

TEST_CASE("List - 混合删除操作", "[List][Delete]") {
  List l;
  std::list<int> stdL;

  // 插入元素 [1, 2, 3, 4, 5]
  for (int i = 1; i <= 5; i++) {
    l.push_back(i);
    stdL.push_back(i);
  }

  // 混合删除
  l.pop_front();
  stdL.pop_front(); // [2, 3, 4, 5]
  l.pop_back();
  stdL.pop_back(); // [2, 3, 4]
  l.pop_front();
  stdL.pop_front(); // [3, 4]
  l.pop_back();
  stdL.pop_back(); // [3]

  auto vec = l.print();
  int index = 0;
  for (auto &i : stdL) {
    REQUIRE(vec[index++] == i);
  }
}

TEST_CASE("List - 删除所有元素", "[List][Delete]") {
  List l;

  // 插入元素
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  // 删除所有元素
  l.pop_front();
  l.pop_front();
  l.pop_front();

  auto vec = l.print();
  REQUIRE(vec.empty());
}