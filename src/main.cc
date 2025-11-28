// src/main.cpp
#include "HashMap/hashmap.h"
#include <cstdlib>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
using std::cout;
using std::endl;

int main() {
    if (true) { // 设置为false的时候执行TEST框架
      MySTL::HashMap<std::string, int> map;
      std::cout << map.bucket_count() << std::endl;
      map.insert({"pxl", 22});
      std::cout << map["pxl"] << std::endl;
      map["pxl"] = 23;
      std::cout << map["pxl"] << std::endl;
    } else {
        Catch::Session session;
        const char*    argv[] = {"Container", "[List]"};
        int            argc   = sizeof(argv) / sizeof(argv[0]);
        session.applyCommandLine(argc, argv);
        return session.run();
    }
}