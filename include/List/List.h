#pragma once
#include <vector>
struct ListNode {
  int val;
  ListNode *next;
  ListNode *prev;
  ListNode(int val) : val(val), next(nullptr), prev(nullptr) {}
  
};

class List {
private:
  ListNode *head; // 将prev当作尾节点
  void printHelp(std::vector<int>& vec);
public:
  /**
   * @brief 构造函数
   *
   */
  List() : head(nullptr) {}
  ~List();

  /**
   * @brief 将数据插入链表的尾部
   *
   */
  void push_back(int val);

  /**
   * @brief 将数据插入链表的头部
   *
   */
  void push_front(int val);

  /**
   * @brief 最链表最后面删除节点
   *
   */
  void pop_back();

  /**
   * @brief 从链表前面删除数据
   *
   */
  void pop_front();
  std::vector<int> print();
};