#include "BST/AVLTree.h"
#include "Vector/Vector.h"
#include "catch.hpp"
#include <algorithm>
#include <set>
#include <vector>

// ==================== 基础功能测试 ====================

TEST_CASE("AVLTree - 空树初始状态", "[AVL][Empty]") {
    AVLTree tree;
    std::vector<int> result;
    tree.printOrdTree(result);
    REQUIRE(result.empty());
}

TEST_CASE("AVLTree - 单节点插入", "[AVL][Insert]") {
    AVLTree tree;
    tree.insert(10);
    
    std::vector<int> result;
    tree.printOrdTree(result);
    
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == 10);
}

TEST_CASE("AVLTree - 基础插入操作", "[AVL][Insert]") {
    SECTION("插入递增序列") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {1, 2, 3, 4, 5};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("插入递减序列") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {5, 4, 3, 2, 1};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("插入随机序列") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8, 11, 13, 16, 20};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
}

// ==================== 旋转测试 ====================

TEST_CASE("AVLTree - 左旋转测试", "[AVL][Rotation]") {
    SECTION("简单左旋 - 右右情况") {
        AVLTree tree;
        
        // 构造需要左旋的情况
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);  // 这应该触发左旋
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        // AVL树应该保持平衡，中序遍历仍然有序
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        REQUIRE(result.size() == 3);
    }
}

TEST_CASE("AVLTree - 右旋转测试", "[AVL][Rotation]") {
    SECTION("简单右旋 - 左左情况") {
        AVLTree tree;
        
        // 构造需要右旋的情况
        tree.insert(30);
        tree.insert(20);
        tree.insert(10);  // 这应该触发右旋
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        // AVL树应该保持平衡，中序遍历仍然有序
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        REQUIRE(result.size() == 3);
    }
}

TEST_CASE("AVLTree - 双旋转测试", "[AVL][Rotation]") {
    SECTION("左右旋转") {
        AVLTree tree;
        
        // 构造需要左右旋转的情况
        tree.insert(30);
        tree.insert(10);
        tree.insert(20);  // 这应该触发左右旋转
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        REQUIRE(result.size() == 3);
    }
    
    SECTION("右左旋转") {
        AVLTree tree;
        
        // 构造需要右左旋转的情况
        tree.insert(10);
        tree.insert(30);
        tree.insert(20);  // 这应该触发右左旋转
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        REQUIRE(result.size() == 3);
    }
}

// ==================== 删除操作测试 ====================

TEST_CASE("AVLTree - 单节点删除", "[AVL][Delete]") {
    AVLTree tree;
    tree.insert(10);
    tree.erase(10);
    
    std::vector<int> result;
    tree.printOrdTree(result);
    REQUIRE(result.empty());
}

TEST_CASE("AVLTree - 基础删除操作", "[AVL][Delete]") {
    SECTION("删除叶子节点") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 18};
        for (int val : insertData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        // 删除叶子节点
        tree.erase(3);
        stdSet.erase(3);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("删除有一个子节点的节点") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 18, 1};
        for (int val : insertData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        // 删除有一个子节点的节点
        tree.erase(3);
        stdSet.erase(3);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("删除有两个子节点的节点") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8};
        for (int val : insertData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        // 删除有两个子节点的节点
        tree.erase(5);
        stdSet.erase(5);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("删除根节点") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 18};
        for (int val : insertData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        // 删除根节点
        tree.erase(10);
        stdSet.erase(10);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
}

TEST_CASE("AVLTree - 删除后平衡测试", "[AVL][Delete][Balance]") {
    SECTION("删除导致左旋") {
        AVLTree tree;
        
        // 构造一个平衡的树
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 20, 18, 25};
        for (int val : insertData) {
            tree.insert(val);
        }
        
        // 删除左子树的节点，可能导致右子树过高
        tree.erase(3);
        tree.erase(5);
        tree.erase(7);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        // 树应该仍然保持平衡和有序
        REQUIRE(std::is_sorted(result.begin(), result.end()));
    }
    
    SECTION("删除导致右旋") {
        AVLTree tree;
        
        // 构造一个平衡的树
        std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 20, 1, 4};
        for (int val : insertData) {
            tree.insert(val);
        }
        
        // 删除右子树的节点，可能导致左子树过高
        tree.erase(12);
        tree.erase(15);
        tree.erase(20);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        // 树应该仍然保持平衡和有序
        REQUIRE(std::is_sorted(result.begin(), result.end()));
    }
}

// ==================== 边界情况测试 ====================

TEST_CASE("AVLTree - 边界情况", "[AVL][Edge]") {
    SECTION("删除不存在的元素") {
        AVLTree tree;
        
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        
        // 删除不存在的元素，应该不影响树
        tree.erase(100);
        tree.erase(-100);
        tree.erase(7);  // 不存在
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected = {5, 10, 15};
        REQUIRE(result == expected);
    }
    
    SECTION("重复插入相同元素") {
        AVLTree tree;
        
        tree.insert(10);
        tree.insert(10);  // 重复插入
        tree.insert(10);  // 再次重复
        tree.insert(5);
        tree.insert(5);   // 重复插入
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        // 根据AVL树的实现，可能允许重复值或忽略重复值
        // 这里假设忽略重复值
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        // 如果允许重复，可以检查具体的重复次数
    }
    
    SECTION("空树删除操作") {
        AVLTree tree;
        
        // 在空树上删除元素，应该不会崩溃
        tree.erase(10);
        tree.erase(0);
        tree.erase(-10);
        
        std::vector<int> result;
        tree.printOrdTree(result);
        REQUIRE(result.empty());
    }
}

// ==================== 大量数据测试 ====================

TEST_CASE("AVLTree - 大量数据测试", "[AVL][Performance]") {
    SECTION("大量顺序插入") {
        AVLTree tree;
        std::set<int> stdSet;
        
        const int testSize = 1000;
        
        for (int i = 1; i <= testSize; i++) {
            tree.insert(i);
            stdSet.insert(i);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
        REQUIRE(result.size() == testSize);
    }
    
    SECTION("大量逆序插入") {
        AVLTree tree;
        std::set<int> stdSet;
        
        const int testSize = 1000;
        
        for (int i = testSize; i >= 1; i--) {
            tree.insert(i);
            stdSet.insert(i);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
        REQUIRE(result.size() == testSize);
    }
    
    SECTION("大量随机插入") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData;
        for (int i = 1; i <= 500; i++) {
            testData.push_back(i);
        }
        
        // 随机打乱
        std::random_shuffle(testData.begin(), testData.end());
        
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
}

// ==================== 复杂操作序列测试 ====================

TEST_CASE("AVLTree - 复杂操作序列", "[AVL][Complex]") {
    SECTION("交替插入删除") {
        AVLTree tree;
        std::set<int> stdSet;
        
        // 复杂的插入删除序列
        std::vector<std::pair<bool, int>> operations = {
            {true, 10},   // insert 10
            {true, 5},    // insert 5
            {true, 15},   // insert 15
            {true, 3},    // insert 3
            {false, 5},   // erase 5
            {true, 7},    // insert 7
            {true, 12},   // insert 12
            {false, 3},   // erase 3
            {true, 18},   // insert 18
            {false, 15},  // erase 15
            {true, 20},   // insert 20
            {false, 10},  // erase 10
        };
        
        for (auto &op : operations) {
            if (op.first) {
                tree.insert(op.second);
                stdSet.insert(op.second);
            } else {
                tree.erase(op.second);
                stdSet.erase(op.second);
            }
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("构建后批量删除") {
        AVLTree tree;
        std::set<int> stdSet;
        
        // 先构建一个较大的树
        std::vector<int> insertData = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};
        for (int val : insertData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        // 批量删除
        std::vector<int> deleteData = {12, 75, 31, 87, 6, 43};
        for (int val : deleteData) {
            tree.erase(val);
            stdSet.erase(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
}

// ==================== 特殊值测试 ====================

TEST_CASE("AVLTree - 特殊值测试", "[AVL][Special]") {
    SECTION("负数测试") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {-10, -5, -15, -3, -7, -12, -18};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("零值测试") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {0, -5, 5, -3, 3, -1, 1};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
    
    SECTION("极值测试") {
        AVLTree tree;
        std::set<int> stdSet;
        
        std::vector<int> testData = {INT_MAX, INT_MIN, INT_MAX-1, INT_MIN+1, 0};
        for (int val : testData) {
            tree.insert(val);
            stdSet.insert(val);
        }
        
        std::vector<int> result;
        tree.printOrdTree(result);
        
        std::vector<int> expected(stdSet.begin(), stdSet.end());
        REQUIRE(result == expected);
    }
}

// ==================== 删除所有元素测试 ====================

TEST_CASE("AVLTree - 删除所有元素", "[AVL][Delete][All]") {
    AVLTree tree;
    
    std::vector<int> insertData = {10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8, 11, 13, 16, 20};
    for (int val : insertData) {
        tree.insert(val);
    }
    
    // 删除所有元素
    for (int val : insertData) {
        tree.erase(val);
    }
    
    std::vector<int> result;
    tree.printOrdTree(result);
    REQUIRE(result.empty());
}