#pragma once
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>
namespace MyList {
template <typename T>
class List {
public:
    /**
     * @brief 构造函数
     *
     */
    List();
    ~List();

    /**
     * @brief 将数据插入链表的尾部
     *
     */
    void push_back(const T& val);

    /**
     * @brief 将数据插入链表的头部
     *
     */
    void push_front(const T& val);

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

    size_t size() const;

private:
    struct ListNode {
        T         val;
        ListNode* next;
        ListNode(const T& val) : val(val), next(nullptr) {}
    };
    ListNode* _head;
    size_t    _size;
};

template <typename T>
List<T>::List() : _head(nullptr) {}

template <typename T>
List<T>::~List() {
    if (_head == nullptr) {
        return;
    }
    while (_head) {
        ListNode* cur = _head;
        delete cur;
        _head = _head->next;
    }
    _head = nullptr;
    _size = 0;
}

template <typename T>
size_t List<T>::size() const {
    return _size;
}

template <typename T>
void List<T>::push_front(const T& val) {
    ListNode* newNode = new ListNode(val);
    if (_head == nullptr) {
        _head = newNode;
        return;
    }
    newNode->next = _head;
    _head         = newNode;
}

template <typename T>
void List<T>::push_back(const T& val) {
    ListNode* newNode = new ListNode(val);
    if (_head == nullptr) {
        _head == newNode;
        return;
    }
    ListNode* cur = _head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = newNode;
}

} // namespace MyList