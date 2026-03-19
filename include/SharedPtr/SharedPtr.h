#pragma once
#include <cstddef>
#include <utility>

/*
 * ============================================================================
 *  SharedPtr - 共享所有权智能指针
 * ============================================================================
 *
 *  这个头文件只提供了最基本的骨架。
 *  具体需要哪些成员函数、怎么实现，请参考 SharedPtr_Lab.md 文档。
 *
 *  你的任务:
 *    1. 阅读 Lab 文档，理解引用计数的原理
 *    2. 根据文档中的要求，在这个文件中添加成员函数声明和实现
 *    3. 运行测试用例验证你的实现
 *
 * ============================================================================
 */

namespace MySTL {

template <typename T>
class SharedPtr {
public:
    // ======================== 基础函数（已给出签名） ========================

    // 默认构造函数
    SharedPtr();

    // 参数构造函数，接管裸指针
    explicit SharedPtr(T* ptr);

    // 析构函数
    ~SharedPtr();

    // ======================== 以下函数需要你自己实现 ========================
    // 请阅读 SharedPtr_Lab.md 文档，理解每个函数应该做什么
    // 函数签名已给出，函数体是空的 TODO，等你来填写

    // --- Part 2: 拷贝语义 ---
    SharedPtr(const SharedPtr& other);
    SharedPtr& operator=(const SharedPtr& other);

    // --- Part 3: 移动语义 ---
    SharedPtr(SharedPtr&& other) noexcept;
    SharedPtr& operator=(SharedPtr&& other) noexcept;

    // --- Part 4: 指针操作 ---
    T& operator*() const;
    T* operator->() const;
    T* get() const;

    // --- Part 5: 辅助函数 ---
    size_t   use_count() const;
    void     reset(T* ptr = nullptr);
    explicit operator bool() const;

private:
    T*      _ptr;       // 指向管理的对象
    size_t* _ref_count; // 指向引用计数（堆上分配）
};

// ============================================================================
//  以下是已给出的基础函数实现 - 仅供参考，你也可以修改它们
// ============================================================================

// --- 默认构造函数 ---
template <typename T>
SharedPtr<T>::SharedPtr() : _ptr(nullptr), _ref_count(nullptr) {}

// --- 参数构造函数 ---
template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : _ptr(ptr), _ref_count(ptr ? new size_t(1) : nullptr) {}

// --- 析构函数 ---
// 提示: 这是 SharedPtr 最关键的函数之一，你需要自己完成逻辑
// 思考: 什么时候应该 delete _ptr？什么时候只需要减少引用计数？
template <typename T>
SharedPtr<T>::~SharedPtr() {
    // TODO: 你来实现
    // 想一想:
    //   - 如果 _ref_count 为 nullptr，需要做什么？
    //   - 如果引用计数减到 0，需要做什么？
    //   - 如果引用计数还大于 0，需要做什么？
    if (*_ref_count > 0) {
        *_ref_count--;
    } else {
        // count = 0
        delete _ref_count;
        delete _ptr;
    }
}

// ============================================================================
//  Part 2: 拷贝语义 - TODO: 你来实现
// ============================================================================

// --- 拷贝构造函数 ---
// 思考: 拷贝时 _ptr 和 _ref_count 怎么处理？引用计数要变化吗？
template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _ref_count(other._ref_count) {
    *_ref_count++;
}

// --- 拷贝赋值运算符 ---
// 思考: 赋值前要先处理自己当前管理的资源（类似析构逻辑）
template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
    if (&other == this) {
        return;
    }
    _ptr       = other._ptr;
    _ref_count = other._ref_count;
    return *this;
}

// ============================================================================
//  Part 3: 移动语义 - TODO: 你来实现
// ============================================================================

// --- 移动构造函数 ---
// 思考: 移动和拷贝的区别？移动时引用计数需要变化吗？
template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : _ptr(other._ptr), _ref_count(other._ref_count) {
    other._ptr       = nullptr;
    other._ref_count = 0;
}

// --- 移动赋值运算符 ---
template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
    if (&other == this) {
        return;
    }
    return *this;
}

// ============================================================================
//  Part 4: 指针操作 - TODO: 你来实现
// ============================================================================

template <typename T>
T& SharedPtr<T>::operator*() const {
    // TODO: 返回 _ptr 指向的对象的引用
    return *_ptr;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    // TODO: 返回 _ptr
    return _ptr;
}

template <typename T>
T* SharedPtr<T>::get() const {
    // TODO: 返回 _ptr
    return _ptr;
}

// ============================================================================
//  Part 5: 辅助函数 - TODO: 你来实现
// ============================================================================

// --- use_count ---
// 思考: 如果 _ref_count 为 nullptr，应该返回什么？
template <typename T>
size_t SharedPtr<T>::use_count() const {
    // TODO
    return 0;
}

// --- reset ---
// 思考: 前半部分逻辑和析构函数很像，能否复用思路？
template <typename T>
void SharedPtr<T>::reset(T* ptr) {
    // TODO
}

// --- operator bool ---
template <typename T>
SharedPtr<T>::operator bool() const {
    // TODO: 返回 _ptr 是否非空
    return false;
}

// ============================================================================
//  Part 6: make_shared（选做进阶）
// ============================================================================

/*
 * template <typename T, typename... Args>
 * SharedPtr<T> make_shared(Args&&... args) {
 *     // TODO: 类似 make_unique，用完美转发创建对象
 * }
 */

} // namespace MySTL
