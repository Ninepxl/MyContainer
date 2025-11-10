#pragma once
#include <array>
#include <cstddef>
#include <ostream>
namespace MyMath {
class vec4 {
private:
  std::array<float, 4> data;

public:
  vec4() = default;
  vec4(float x, float y, float z, float w);
  vec4(const vec4 &v2);
  vec4(vec4 &&other) noexcept;
  ~vec4() = default;

  vec4 &operator=(const vec4 &v2);
  vec4 &operator=(vec4 &&v2) noexcept;

  float &operator[](const std::size_t index);
  const float &operator[](const std::size_t index) const;

  vec4 &operator+=(const vec4 &v2);
  vec4 &operator-=(const vec4 &v2);
  vec4 &operator*=(float c); // multiplication by a scalar
  vec4 &operator/=(float c); // division by a scalar

  vec4 operator+(const vec4 &v2) const;
  vec4 operator-(const vec4 &v2) const;
  vec4 operator*(float c) const; // multiplication by a scalar
  vec4 operator/(float c) const; // division by a scalar
};

/// Dot Product
float dot(const vec4 &v1, const vec4 &v2);

/// Cross Product
vec4 cross(const vec4 &v1,
           const vec4 &v2); // Compute the result of v1 x v2 using only their X,
                            // Y, and Z elements. In other words, treat v1 and
                            // v2 as 3D vectors, not 4D vectors. The fourth
                            // element of the resultant vector should be 0.

/// Returns the geometric length of the input vector
float length(const vec4 &v);

/// Scalar Multiplication (c * v)
vec4 operator*(float c, const vec4 &v);

vec4 normalize(const vec4 &v);

/// Prints the vector to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const vec4 &v);
} // namespace MyMath