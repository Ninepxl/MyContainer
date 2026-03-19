#pragma once
#include <cstddef>
#include <utility>

/*
 * ============================================================================
 *  UniquePtr - 独占所有权智能指针
 * ============================================================================
 *
 *  【什么是 UniquePtr？】
 *  UniquePtr 是一个智能指针，它"独占"所管理的堆内存对象。
 *  同一时刻只能有一个 UniquePtr 指向某个对象，不允许拷贝，只允许移动。
 *  当 UniquePtr 被销毁时（离开作用域），它会自动 delete 所管理的指针。
 *
 *  【核心概念】
 *  1. RAII (Resource Acquisition Is Initialization)
 *     - 资源在构造时获取，在析构时释放
 *     - UniquePtr 的构造函数接管裸指针，析构函数负责 delete
 *
 *  2. 移动语义 (Move Semantics)
 *     - UniquePtr 不能被拷贝（拷贝构造和拷贝赋值被 delete）
 *     - 但可以通过 std::move() 转移所有权
 *     - 移动后，原来的 UniquePtr 变为 nullptr
 *
 *  3. 独占所有权 (Exclusive Ownership)
 *     - 任何时刻，一个堆对象最多只被一个 UniquePtr 管理
 *     - 这从根本上避免了 double free 问题
 *
 *  【你需要实现的函数列表】
 *     构造 / 析构:
 *       - 默认构造函数        UniquePtr()
 *       - 参数构造函数        UniquePtr(T* ptr)
 *       - 析构函数            ~UniquePtr()
 *
 *     禁止拷贝:
 *       - 拷贝构造函数        UniquePtr(const UniquePtr&) = delete
 *       - 拷贝赋值运算符      operator=(const UniquePtr&) = delete
 *
 *     移动语义:
 *       - 移动构造函数        UniquePtr(UniquePtr&& other)
 *       - 移动赋值运算符      operator=(UniquePtr&& other)
 *
 *     指针操作:
 *       - 解引用              operator*()
 *       - 箭头运算符          operator->()
 *       - 获取裸指针          get()
 *       - 释放所有权          release()
 *       - 重置指针            reset(T* ptr = nullptr)
 *
 *     布尔转换:
 *       - operator bool()     判断是否持有有效指针
 *
 *  【参考资料】
 *  - cppreference std::unique_ptr:
 *    https://en.cppreference.com/w/cpp/memory/unique_ptr
 *  - C++ Move Semantics 详解:
 *    https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/
 *  - RAII 概念:
 *    https://en.cppreference.com/w/cpp/language/raii
 *
 * ============================================================================
 */

namespace MySTL {

template <typename T>
class UniquePtr {
public:
    // ======================== 构造与析构 ========================

    /*
     * 默认构造函数
     * 初始化内部指针为 nullptr
     * 提示: 使用初始化列表
     */
    UniquePtr();

    /*
     * 参数构造函数 (explicit)
     * 接管一个裸指针的所有权
     *
     * 为什么要用 explicit？
     *   防止隐式转换，比如: UniquePtr<int> p = new int(5);
     *   这种写法容易造成混淆，应该写成: UniquePtr<int> p(new int(5));
     *
     * @param ptr - 要管理的裸指针
     */
    explicit UniquePtr(T* ptr);

    /*
     * 析构函数
     * 释放所管理的内存
     *
     * 提示: 用 delete 释放 _ptr 指向的内存
     * 思考: 需要判断 _ptr 是否为 nullptr 吗？
     *       (答案: delete nullptr 是安全的，C++ 标准保证)
     */
    ~UniquePtr();

    // ======================== 禁止拷贝 ========================

    /*
     * 拷贝构造函数 - 已禁用
     *
     * 为什么禁止拷贝？
     *   如果允许拷贝，两个 UniquePtr 会指向同一块内存，
     *   当它们各自析构时，同一块内存会被 delete 两次 (double free)！
     *
     * 语法: = delete 告诉编译器这个函数被删除，任何调用都会编译报错
     */
    UniquePtr(const UniquePtr& other) = delete;

    /*
     * 拷贝赋值运算符 - 已禁用
     * 理由同上
     */
    UniquePtr& operator=(const UniquePtr& other) = delete;

    // ======================== 移动语义 ========================

    /*
     * 移动构造函数
     * 从 other 那里"偷走"指针的所有权
     *
     * 步骤:
     *   1. 把 other._ptr 的值赋给自己的 _ptr
     *   2. 把 other._ptr 设为 nullptr（other 不再拥有这块内存）
     *
     * 使用场景:
     *   UniquePtr<int> p1(new int(42));
     *   UniquePtr<int> p2(std::move(p1));  // p1 -> nullptr, p2 -> 42
     *
     * @param other - 被移动的 UniquePtr (右值引用)
     */
    UniquePtr(UniquePtr&& other) noexcept;

    /*
     * 移动赋值运算符
     * 释放自己当前管理的内存，然后从 other 那里接管所有权
     *
     * 步骤:
     *   1. 检查自赋值 (this != &other)
     *   2. delete 自己当前的 _ptr
     *   3. 把 other._ptr 赋给自己
     *   4. 把 other._ptr 设为 nullptr
     *   5. 返回 *this
     *
     * 思考: 为什么要检查自赋值？
     *   如果 a = std::move(a)，不检查的话会先 delete 自己的指针，
     *   然后再把已经被 delete 的指针赋给自己 —— 悬空指针！
     *
     * @param other - 被移动的 UniquePtr
     * @return 返回 *this 的引用，支持链式赋值
     */
    UniquePtr& operator=(UniquePtr&& other) noexcept;

    // ======================== 指针操作 ========================

    /*
     * 解引用运算符
     * 返回所管理对象的引用
     *
     * 类比: 和裸指针的 *ptr 行为一致
     * 注意: 如果 _ptr 为 nullptr，解引用是未定义行为 (UB)
     *
     * @return T& - 对象的引用
     */
    T& operator*() const;

    /*
     * 箭头运算符
     * 返回所管理的裸指针，用于访问对象的成员
     *
     * 使用场景:
     *   struct Foo { int x; };
     *   UniquePtr<Foo> p(new Foo{42});
     *   p->x;  // 等价于 (*p).x
     *
     * @return T* - 裸指针
     */
    T* operator->() const;

    /*
     * 获取裸指针
     * 返回内部管理的裸指针，但不转移所有权
     *
     * 注意: 调用者不应该 delete 返回的指针！
     *       所有权仍然属于 UniquePtr
     *
     * @return T* - 裸指针
     */
    T* get() const;

    /*
     * 释放所有权
     * 返回裸指针，并放弃对它的管理（不 delete）
     *
     * 步骤:
     *   1. 保存当前 _ptr 到一个临时变量
     *   2. 将 _ptr 设为 nullptr
     *   3. 返回临时变量
     *
     * 调用后: UniquePtr 变为空，调用者负责管理返回的指针
     *
     * @return T* - 被释放的裸指针，调用者需要自行 delete
     */
    T* release();

    /*
     * 重置指针
     * 释放当前管理的内存，然后接管新的指针
     *
     * 步骤:
     *   1. delete 当前的 _ptr
     *   2. 将 _ptr 设为 ptr（默认为 nullptr）
     *
     * 使用场景:
     *   p.reset();              // 释放并置空
     *   p.reset(new int(100));  // 释放旧的，接管新的
     *
     * @param ptr - 新的裸指针，默认为 nullptr
     */
    void reset(T* ptr = nullptr);

    // ======================== 布尔转换 ========================

    /*
     * 布尔转换运算符
     * 判断 UniquePtr 是否持有有效指针（非 nullptr）
     *
     * 为什么用 explicit？
     *   防止 UniquePtr 被隐式转换为 bool 参与算术运算等
     *   explicit 后只能在 if/while 等布尔上下文中隐式转换
     *
     * 使用场景:
     *   UniquePtr<int> p(new int(42));
     *   if (p) { ... }       // OK, 在布尔上下文中
     *   bool b = (bool)p;    // OK, 显式转换
     *
     * @return true 如果持有非空指针, false 如果为 nullptr
     */
    explicit operator bool() const;

private:
    T* _ptr; // 内部管理的裸指针
};

// ============================================================================
//  以下是函数实现区域 - 请你来完成每个函数的实现！
//  模板类的实现必须放在头文件中（编译器需要在编译时看到完整定义）
// ============================================================================

// --- 默认构造函数 ---
template <typename T>
UniquePtr<T>::UniquePtr() : _ptr(nullptr) {}

// --- 参数构造函数 ---
template <typename T>
UniquePtr<T>::UniquePtr(T* ptr) : _ptr(ptr) {}

// --- 析构函数 ---
template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete _ptr;
}

// --- 移动构造函数 ---
template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept {
    _ptr       = other._ptr;
    other._ptr = nullptr;
}

// --- 移动赋值运算符 ---
template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if (&other == this) {
        return *this;
    }
    delete _ptr;
    _ptr       = other._ptr;
    other._ptr = nullptr;
    return *this;
}

// --- 解引用运算符 ---
template <typename T>
T& UniquePtr<T>::operator*() const {
    return *_ptr;
}

// --- 箭头运算符 ---
template <typename T>
T* UniquePtr<T>::operator->() const {
    return _ptr;
}

// --- 获取裸指针 ---
template <typename T>
T* UniquePtr<T>::get() const {
    return _ptr;
}

// --- 释放所有权 ---
template <typename T>
T* UniquePtr<T>::release() {
    auto* temp = _ptr;
    _ptr       = nullptr;
    return temp;
}

// --- 重置指针 ---
template <typename T>
void UniquePtr<T>::reset(T* ptr) {
    delete _ptr;
    _ptr = ptr;
}

// --- 布尔转换 ---
template <typename T>
UniquePtr<T>::operator bool() const {
    return _ptr;
}

// ============================================================================
//  make_unique - 工厂函数（选做，进阶挑战）
// ============================================================================

/*
 * make_unique 函数模板
 * 用给定的参数直接在堆上构造对象，返回 UniquePtr
 *
 * 这是一个"完美转发"的经典应用:
 *   - Args&&... args 是万能引用 (universal reference)
 *   - std::forward<Args>(args)... 保持参数的左值/右值属性
 *
 * 使用场景:
 *   auto p = MySTL::make_unique<int>(42);
 *   auto p2 = MySTL::make_unique<std::string>("hello");
 *
 * 为什么推荐用 make_unique 而不是 UniquePtr<T>(new T(...))?
 *   1. 更安全: 避免裸 new 出现在代码中
 *   2. 异常安全: 在某些复杂表达式中避免内存泄漏
 *
 * 提示: 实现只需要一行: return UniquePtr<T>(new T(std::forward<Args>(args)...));
 */
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    // TODO: 用 new T(...) 创建对象，用完美转发传递参数
    提示: return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

} // namespace MySTL
