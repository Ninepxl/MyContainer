// src/main.cpp
#include <cstdlib>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
using std::cout;
using std::endl;

// static int s_AllocationCount = 0;
// void *operator new(size_t size) {
//   std::cout << "Allocated " << size << " bytes\n";
//   s_AllocationCount++;
//   return malloc(size);
// }

struct array {
  int a[10];
};

const int f() {
  return 10;
}

int main() {
  if (true) { // 设置为false的时候执行TEST框架
  } else {
    Catch::Session session;
    const char *argv[] = {"Container", "[List]"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    session.applyCommandLine(argc, argv);
    return session.run();
  }
}