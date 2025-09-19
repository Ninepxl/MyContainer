#include "Vector/Vector.h"
#include <cstddef>

Vector::Vector() {
  this->_capacity = 5;
  this->element = new int[this->_capacity];
  this->_size = 0;
}

Vector::Vector(int n, int value) {
  _capacity = n;
  this->element = new int[this->_capacity];
  _size = _capacity;
  for (int i = 0; i < n; i++) {
    element[i] = value;
  }
}

Vector::~Vector() { delete[] this->element; }

size_t Vector::size() const { return this->_size; }

void Vector::push_back(int value) {
  if (_size == _capacity) {
    // 扩容
    expand();
  }
  element[_size] = value;
  _size++;
}

int& Vector::operator[](size_t index) { return element[index]; }
const int &Vector::operator[](size_t index) const { return element[index]; }
int Vector::at(size_t index) {
  if (index > size() - 1) {
    throw "Index Scope Error";
  }
  return element[index];
}

void Vector::clear() { _size = 0; }

void Vector::inster(size_t index, int value) {
  if (_size == _capacity) {
    expand();
  }
  for (int i = _size; i > index; i--) {
    element[i] = element[i - 1];
  }
  element[index] = value;
}

void Vector::expand() {
  _capacity *= 2;
  int *newElem = new int[_capacity];
  for (int i = 0; i < _size; i++) {
    newElem[i] = element[i];
  }
  delete[] element;
  element = newElem;
}