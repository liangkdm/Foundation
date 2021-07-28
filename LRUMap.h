#pragma once

#include <list>
#include <unordered_map>

template <typename K, typename V>
class LRUMap {
public:
    typedef typename std::unordered_map<K, V>::iterator iterator;
    typedef typename std::unordered_map<K, V>::key_type key_type;
    typedef typename std::unordered_map<K, V>::value_type value_type;

    LRUMap()
        : _capacity(1024) {
    }

    explicit LRUMap(size_t capacity) {
        _capacity = capacity > 0 ? capacity : 1024;
    }

    ~LRUMap() {
    }

    size_t size() const {
        return _kv.size();
    }
    bool empty() const {
        return this->size() == 0;
    }
    iterator begin() const {
        return ((LRUMap *)this)->_kv.begin();
    }
    iterator end() const {
        return ((LRUMap *)this)->_kv.end();
    }

    iterator find(const key_type &key) {
        iterator it = _kv.find(key);
        if (it != _kv.end() && _kl.front() != key) {
            auto ki = _ki.find(key);
            _kl.splice(_kl.begin(), _kl, ki->second); //当前key value位最活跃状态， 将当前key置位到查询keylist最前
            ki->second = _kl.begin();
        }
        return it;
    }

    // The key is not inserted if it already exists.
    template <typename Key, typename Val> 
    void insert(Key &&key, Val &&value) {
        if (_kv.size() >= _capacity) {
            K k = _kl.back();
            _kl.pop_back();
            _kv.erase(k);
            _ki.erase(k);
        }
        auto r = _kv.insert(value_type(std::forward<Key>(key), std::forward<Val>(value)));
        if (r.second) {
            //当前key value位最活跃状态， 将当前key置位到查询keylist最前
            _kl.push_front(key);
            _ki[ key ] = _kl.begin();
        }
    }

    void erase(iterator it) {
        if (it != _kv.end()) {
            auto ki = _ki.find(it->first);
            _kl.erase(ki->second);
            _ki.erase(ki);
            _kv.erase(it);
        }
    }

    void erase(const key_type &key) {
        this->erase(_kv.find(key));
    }

    void clear() {
        _kv.clear();
        _ki.clear();
        _kl.clear();
    }

    void swap(LRUMap &x) noexcept {
        _kv.swap(x._kv);
        _ki.swap(x._ki);
        _kl.swap(x._kl);
        std::swap(_capacity, x._capacity);
    }

    void swap(LRUMap &&x) noexcept {
        x.swap(*this);
    }

private:
    std::unordered_map<K, V> _kv;
    std::unordered_map<K, typename std::list<K>::iterator> _ki;
    std::list<K> _kl; // key list
    size_t _capacity; // max capacity
};
