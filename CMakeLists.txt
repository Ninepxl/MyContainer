
cmake_minimum_required(VERSION 3.20)
project(Container)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置头文件搜索路径
# 这一步很关键，会告诉编译器去哪里找头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 创建可执行文件
add_executable(Container 
    src/main.cc
    src/Vector/Vector.cc 
    src/BST/set.cc
    src/BST/AVLTree.cc
    test/VectorTest.cc
)

# 为目标设置额外的头文件路径
target_include_directories(Container 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)