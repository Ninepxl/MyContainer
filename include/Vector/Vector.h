#include <cstddef>
// 支持int 类型的Vector
// class 的所有构造函数，const 迭代器
class Vector {
public:
  // 初始化一个新的空的Vector
  Vector();
  Vector(int n, int value);
  ~Vector();
  size_t size() const;
  // Adds a new value to the end of this vector
  void push_back(int vlaue);
  // Removes all elements from this vector
  void clear();
  // inserts value into this vector at the specified index
  void inster(size_t index, int value);
  // returns the element at the specified index in this vector
  int at(size_t index);
  int &operator[](size_t index);
  const int &operator[](size_t index) const;

private:
  int *element;
  size_t _size;
  size_t _capacity;
  void expand();
};