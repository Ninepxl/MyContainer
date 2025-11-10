#include "List/List.h"
#include <vector>

namespace MyList {
void List::push_back(int val) {
  ListNode *newNode = new ListNode(val);
  if (this->head == nullptr) {
    head = newNode;
    head->prev = head;
    return;
  }
  ListNode *tail = head->prev;
  newNode->prev = tail;
  tail->next = newNode;
  head->prev = newNode;
}

void List::push_front(int val) {
  if (head == nullptr) {
    head = new ListNode(val);
    head->prev = head;
    return;
  }
  ListNode *newNode = new ListNode(val);
  ListNode *tail = head->prev;
  newNode->next = head;
  head->prev = newNode;
  head = newNode;
  head->prev = tail;
}

void List::pop_back() {
  // 如果链表只有一个节点
  if (head->next == nullptr) {
    delete head->prev;
    head = nullptr;
    return;
  }

  ListNode *endNode = head->prev;
  head->prev = endNode->prev;
  endNode->prev->next = nullptr;
  delete endNode;
}

void List::pop_front() {
  if (head->next == nullptr) {
    delete head->prev;
    head = nullptr;
    return;
  }
  ListNode *temp = head;
  head->next->prev = temp->prev;
  head = head->next;
  delete temp;
}

List::~List() {
  while (head) {
    ListNode *node = head;
    head = head->next;
    delete node;
  }
}

void List::printHelp(std::vector<int> &vec) {
  ListNode *cur = this->head;
  while (cur) {
    vec.push_back(cur->val);
    cur = cur->next;
  }
}

std::vector<int> List::print() {
  std::vector<int> vec;
  printHelp(vec);
  return vec;
}

} // namespace MyList