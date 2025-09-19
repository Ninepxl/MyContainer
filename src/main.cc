// src/main.cpp
#include <cstddef>
#include <cstdlib>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "Vector/Vector.h"
#include "catch.hpp"
using std::cout;
using std::endl;

// static int s_AllocationCount = 0;
// void *operator new(size_t size) {
//   std::cout << "Allocated " << size << " bytes\n";
//   s_AllocationCount++;
//   return malloc(size);
// }

int main() {
  if (false) { // 设置为false的时候执行TEST框架
    // std::vector<int> vec;
    // for (int i = 1; i <= 5; i++) {
    //   vec.push_back(i);
    // }
    // cout << s_AllocationCount << " " << "allocation" << endl;
    // Vector v;
    // v[0] = 1;
    // cout << v[0] << endl;
  } else {
    Catch::Session session;
    return session.run();
  }
}