#include "Vector/SimpleVector.h"
#include <cstddef>

SimpleVector::SimpleVector() {
  this->_capacity = 5;
  this->element = new int[this->_capacity];
  this->_size = 0;
}

SimpleVector::SimpleVector(int n, int value) {
  _capacity = n;
  this->element = new int[this->_capacity];
  _size = _capacity;
  for (int i = 0; i < n; i++) {
    element[i] = value;
  }
}

SimpleVector::~SimpleVector() { delete[] this->element; }

size_t SimpleVector::size() const { return this->_size; }

void SimpleVector::push_back(int value) {
  if (_size == _capacity) {
    // 扩容
    expand();
  }
  element[_size] = value;
  _size++;
}

int& SimpleVector::operator[](size_t index) { return element[index]; }
const int &SimpleVector::operator[](size_t index) const { return element[index]; }
int SimpleVector::at(size_t index) {
  if (index > size() - 1) {
    throw "Index Scope Error";
  }
  return element[index];
}

void SimpleVector::clear() { _size = 0; }

void SimpleVector::inster(size_t index, int value) {
  if (_size == _capacity) {
    expand();
  }
  for (int i = _size; i > index; i--) {
    element[i] = element[i - 1];
  }
  element[index] = value;
}

void SimpleVector::expand() {
  _capacity *= 2;
  int *newElem = new int[_capacity];
  for (int i = 0; i < _size; i++) {
    newElem[i] = element[i];
  }
  delete[] element;
  element = newElem;
}