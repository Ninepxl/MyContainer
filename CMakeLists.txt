
cmake_minimum_required(VERSION 3.20)
project(Container)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
# 设置头文件搜索路径
# 这一步很关键，会告诉编译器去哪里找头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 创建可执行文件
add_executable(Container 
    src/main.cc
    src/Vector/SimpleVector.cc 
    src/BST/set.cc
    src/BST/AVLTree.cc
)

# 为目标设置额外的头文件路径
target_include_directories(Container 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# # UniquePtr 测试 (独立可执行文件，方便单独运行)
# add_executable(UniquePtrTest
#     test/UniquePtrTest.cc
# )
# target_include_directories(UniquePtrTest
#     PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
# target_compile_definitions(UniquePtrTest PRIVATE CATCH_CONFIG_MAIN)

# # SharedPtr 测试
# add_executable(SharedPtrTest
#     test/SharedPtrTest.cc
# )
# target_include_directories(SharedPtrTest
#     PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
# target_compile_definitions(SharedPtrTest PRIVATE CATCH_CONFIG_MAIN)