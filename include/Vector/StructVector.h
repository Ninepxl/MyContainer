#pragma once
#include <cstddef>
#include <iostream>
#include <new>
struct MyStruct {
  MyStruct() { std::cout << "普通构造函数" << std::endl; };
  ~MyStruct() { std::cout << "析构函数" << std::endl; }
  MyStruct(const MyStruct &other) { std::cout << "拷贝复制函数" << std::endl; }
  MyStruct& operator=(const MyStruct& oher) = default;
  MyStruct(MyStruct &&other) noexcept {
    std::cout << "移动构造函数" << std::endl;
  }
};

class StructVector {
private:
  MyStruct *_data;
  size_t _size;

public:
  StructVector() { _data = (MyStruct *)::operator new(sizeof(MyStruct)); }
  void push_back(const MyStruct &val) { new (&_data[_size]) MyStruct(val); }
  void push_back(MyStruct&& val) {

  }
};