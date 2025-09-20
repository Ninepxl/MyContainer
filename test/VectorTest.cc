#include "Vector/Vector.h"
#include "BST/AVLTree.h"
#include "catch.hpp"
#include <set>
#include <vector>
// TEST_CASE("Vector: ", "") {
//   Vector v;
//   REQUIRE(v.size() == 0);
//   v.push_back(1);
//   REQUIRE(v.size() == 1);
// }
TEST_CASE("AVL: ", "Insert TEST") {
  std::set<int> s;
  AVLTree avl;
  std::vector<int> setVec;
  std::vector<int> avlVec;
  for (int i = 0; i < 10; i++) {
    s.insert(i);
    avl.insert(i);
  }

  s.erase(9);
  avl.erase(9);
  for (auto &i : s) {
    setVec.push_back(i);
  }
  avl.printOrdTree(avlVec);

  REQUIRE(setVec == avlVec);
}