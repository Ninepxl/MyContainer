#pragma once
#include <cstddef>
#include <utility>
#include <vector>

template <typename T>
class PriorityQueue {
private:
    std::vector<T> data;

public:
    void        push(const T& item);
    T           pop();
    void        empty();
    void        top();
    std::size_t size() const;
};

template <typename T>
void PriorityQueue<T>::push(const T& item) {
    data.push_back(item);
    int insertIndex = data.size() - 1; // 插入到最后一个位置
    while (insertIndex > 0) {
        int parentIndex = (insertIndex - 1) / 2;
        if (data[insertIndex] < data[parentIndex]) {
            break;
        }
        std::swap(data[insertIndex], data[parentIndex]);
        insertIndex = parentIndex;
    }
}

template <typename T>
T PriorityQueue<T>::pop() {
    T item = data[0];                          // 获取根节点的数据
    std::swap(data[0], data[data.size() - 1]); // 根节点和最后一个节点交换方便数组删除
    data.pop_back();
    int insertIndex = 0;
    int n = data.size();
    while (insertIndex * 2 + 1 < n) {
        int childLIndex = insertIndex * 2 + 1;
        int childRIndex = childLIndex + 1;
        int largestIndex = childLIndex;
        if (childRIndex < n && data[childRIndex] > data[childLIndex]) {
            largestIndex = childRIndex;
        }
        if (data[insertIndex] > data[largestIndex]) {
            break;
        }
        std::swap(data[insertIndex], data[largestIndex]);
        insertIndex = largestIndex;
    }
    return item;
}

template <typename T>
size_t PriorityQueue<T>::size() const {
    return data.size();
}