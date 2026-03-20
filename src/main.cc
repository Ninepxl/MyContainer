// src/main.cpp
#include "HashMap/hashmap.h"
#include "PriorityQueue/PriorityQueue.h"
#include <cstdlib>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
using std::cout;
using std::endl;

int main() {
    if (true) { // 设置为false的时候执行TEST框架
       PriorityQueue<int> q;
       q.push(3); 
       q.push(5);
       q.push(2);
       cout << q.size() << endl;
       cout << q.pop() << endl;
    } else {
        Catch::Session session;
        const char*    argv[] = {"Container", "[UniquePtr]"};
        int            argc   = sizeof(argv) / sizeof(argv[0]);
        session.applyCommandLine(argc, argv);
        return session.run();
    }
}