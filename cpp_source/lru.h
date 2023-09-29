#include <assert.h>

#include <iostream>
#include <list>
#include <mutex>
#include <unordered_map>
template <typename K, typename V>
class LRU {
   private:
    size_t size_;
    std::mutex latch_;
    std::list<std::pair<K, V>> list_;
    // typedef typename std::list<std::pair<K, V>>::iterator Iterator;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map_;

   public:
    LRU(size_t sz) : size_(sz) {}
    auto Get(const K &key, V &value) -> bool;
    void Put(K key, V value);

   private:
    auto Evict() -> bool;
    auto InsertItem(const K &, const V &) -> bool;
    auto RemoveItem(const K &) -> bool;
};

template <typename K, typename V>
auto LRU<K, V>::Evict() -> bool {
    if (list_.empty()) {
        return false;
    }
    auto item = list_.back();
    map_.erase(item.first);
    list_.pop_back();
    return true;
}

template <typename K, typename V>
auto LRU<K, V>::InsertItem(const K &key, const V &value) -> bool {
    if (list_.size() == size_) {
        Evict();
    }
    list_.push_front(std::make_pair(key, value));
    map_[key] = list_.begin();
    return true;
}

template <typename K, typename V>
auto LRU<K, V>::RemoveItem(const K &key) -> bool {
    if (map_.find(key) == map_.end()) {
        return false;
    }
    list_.erase(map_[key]);
    map_.erase(key);
    return true;
}

template <typename K, typename V>
auto LRU<K, V>::Get(const K &key, V &value) -> bool {
    std::scoped_lock<std::mutex> lock(latch_);
    if (map_.find(key) == map_.end()) {
        return false;
    }
    value = (*map_[key]).second;
    RemoveItem(key);
    InsertItem(key, value);
    return true;
}

template <typename K, typename V>
void LRU<K, V>::Put(K key, V value) {
    std::scoped_lock<std::mutex> lock(latch_);
    InsertItem(key, value);
}

void lru_test() {
    LRU<int, int> lru_cache(2);
    int k, v;

    lru_cache.Put(1, 1);
    lru_cache.Put(2, 2);
    lru_cache.Get(1, v);
    assert(1 == v);

    lru_cache.Put(3, 3);
    assert(false == lru_cache.Get(2, v));

    lru_cache.Put(4, 4);
    assert(false == lru_cache.Get(1, v));

    lru_cache.Get(3, v);
    assert(3 == v);
    lru_cache.Get(4, v);
    assert(4 == v);
}
