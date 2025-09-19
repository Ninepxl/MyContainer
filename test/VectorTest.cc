#include "Vector/Vector.h"
#include "catch.hpp"
TEST_CASE("Vector: ", "") {
  Vector v;
  REQUIRE(v.size() == 0);
  v.push_back(1);
  REQUIRE(v.size() == 1);
}