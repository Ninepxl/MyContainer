#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

/*
 * 哈希表中的一些术语
 * 桶: 哈希表中数组能够访问到的最大数量
 *
 */
namespace MySTL {
template <typename K, typename M, typename H = std::hash<M>>
class HashMap {
public:
    using value_type = std::pair<const K, M>;
    /*
     * Default constructor
     * Creates an empty HashMap with default number of buckets and hash function.
     *
     * Usage:
     *      HashMap map;
     *      HashMap map{};
     *
     * Complexity: O(B), B = number of buckets
     */
    HashMap();

    /*
     * Constructor with bucket_count and hash function as parameters.
     *
     * Creates an empty HashMap with a specified initial bucket_count and hash
     * funciton. If no hash function provided, default value of H is used.
     *
     * Usage:
     *      HashMap(10) map;
     *      HashMap map(10, [](const K& key) {return key % 10; });
     *      HashMap map{10, [](const K& key) {return key % 10; }};
     *
     * Complexity: O(B), B = number of buckets
     *
     * Notes : what is explicit? Explicit specifies that a constructor
     * cannot perform implicit conversion on the parameters, or use
     * copy-initialization. That's good, as nonsense like the following won't
     * compile:
     *
     * HashMap<int, int> map(1.0);  // double -> int conversion not allowed.
     * HashMap<int, int> map = 1;   // copy-initialization, does not compile.
     */
    explicit HashMap(size_t bucket_count, const H& hash = H());
    ~HashMap();

    /**
     * @brief 返回(K, M)键值对的个数, 时间复杂度O(1)
     *
     * @return size_t
     */
    inline size_t size() const;

    /**
     * @brief 返回哈希表是否为空, 时间复杂度O(1)
     *
     * @return true
     * @return false
     */
    inline bool empty() const;

    /**
     * @brief 返回哈希表的负载因子, 时间复杂度O(1)
     *
     * @return float
     */
    inline float load_factor() const;

    /**
     * @brief 返回哈希表中桶的数量, 时间复杂度O(1)
     *
     * @return size_t
     */
    inline size_t bucket_count() const;

private:
    struct Node {
        value_type val;
        Node*      next;
        Node(const value_type& val = value_type(), Node* next = nullptr) : val(val), next(next) {}
    };

    static constexpr size_t kDefaultBuckets = 10;
    size_t                  _size;
    std::vector<Node*>      _buckets_array;
    H                       hash;
};

/*
Hash Map函数的实现
*/

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap(kDefaultBuckets) {}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t size, const H& hash) : _size(0), _buckets_array(size, nullptr), hash(hash) {}

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::size() const {
    return this->_size;
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::empty() const {
    return this->_size == 0;
}

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::bucket_count() const {
    return this->_buckets_array.size();
}

template <typename K, typename M, typename H>
float HashMap<K, M, H>::load_factor() const {
    return static_cast<float>(this->size() / this->_buckets_array.size());
}

} // namespace MySTL