template <typename T>
struct SpControlBlock {
    T*  m_ptr;
    int m_strongRefCnt;
    explicit SpControlBlock(T* ptr) : m_ptr(ptr), m_strongRefCnt(1) {}
    SpControlBlock(SpControlBlock&&) = delete;
};

template <typename T>
struct SharedPtr {
private:
    SpControlBlock<T>* m_controlBlock;

public:
    explicit SharedPtr(T* ptr) : m_controlBlock(new SpControlBlock(ptr)) {}
    ~SharedPtr() {
        m_controlBlock->m_strongRefCnt--;
        if (m_controlBlock->m_strongRefCnt == 0) {
            delete m_controlBlock->m_ptr;
        }
    }
    SharedPtr(const SharedPtr& other) {}
    SharedPtr(SharedPtr&& other) {}
};