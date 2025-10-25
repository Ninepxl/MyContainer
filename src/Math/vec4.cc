#include "Math/vec4.h"
#include <cmath>
#include <cstddef>
#include <utility>

vec4::vec4(float x, float y, float z, float w) : data({x, y, z, w}) {}
vec4::vec4(const vec4 &v2) : data(v2.data) {}
vec4::vec4(vec4 &&v2) noexcept : data(std::move(v2.data)) {}
vec4 &vec4::operator=(const vec4 &v2) {
  data = v2.data;
  return *this;
}
vec4 &vec4::operator=(vec4 &&v2) noexcept {
  data = std::move(v2.data);
  return *this;
}
float &vec4::operator[](const std::size_t index) { return data[index]; }

const float &vec4::operator[](const std::size_t index) const {
  return data[index];
}

float dot(const vec4 &v1, const vec4 &v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
}

vec4 cross(const vec4 &v1, const vec4 &v2) {
  float x = v1[1] * v2[2] - v1[2] * v2[1];
  float y = v1[0] * v2[2] - v1[2] * v2[0];
  float z = v1[0] * v2[1] - v1[1] * v2[0];
  return vec4(x, y, z, 0);
}

float length(const vec4 &v) {
  return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

vec4 operator*(float c, const vec4 &v) {
  int x = v[0] / c;
  int y = v[1] / c;
  int z = v[2] / c;
  int w = v[3] / c;
  return vec4(x, y, z, w);
}

vec4 normalize(const vec4 &v) {
  auto l = length(v);
  int x = v[0] / l;
  int y = v[1] / l;
  int z = v[2] / l;
  int w = v[3] / l;
  return vec4(x, y, z, w);
}