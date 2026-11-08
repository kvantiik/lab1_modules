
module;

#include <list>
#include <unordered_map>
#include <stdexcept>

export module LRUCache;

import ICache;

export class LRUCache : public ICache {
private:
    std::size_t capacity;
    std::list<std::pair<int, long long>> cache;
    std::unordered_map<int, std::list<std::pair<int, long long>>::iterator> cache_map;

public:
    LRUCache(std::size_t capacity);
    void put(int key, long long value) override;
    long long get(int key) override;
    long long operator[](int key) override;
};

LRUCache::LRUCache(std::size_t capacity) : capacity(capacity) {
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }
}

void LRUCache::put(int key, long long value) {
    auto it = cache_map.find(key);
    if (it != cache_map.end()) {
        it->second->second = value;
        cache.splice(cache.begin(), cache, it->second);
        return;
    }

    if (cache.size() == capacity) {
        cache_map.erase(cache.back().first);
        cache.pop_back();
    }

    cache.emplace_front(key, value);
    cache_map[key] = cache.begin();
}

long long LRUCache::get(int key) {
    auto it = cache_map.find(key);
    if (it == cache_map.end()) {
        throw std::out_of_range("Key not found");
    }

    cache.splice(cache.begin(), cache, it->second);
    return it->second->second;
}

long long LRUCache::operator[](int key) {
    return get(key);
}
