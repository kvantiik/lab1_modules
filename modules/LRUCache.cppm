module;

#include <list>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>

export module LRUCache;

import ICache;

export namespace cache {
    class LRUCache : public ICache {
    public:
        LRUCache(std::size_t capacity);
        void put(std::int32_t key, std::int64_t value) override;
        std::int64_t get(std::int32_t key) override;
        std::int64_t operator[](std::int32_t key) override;
        bool contains(std::int32_t key) override;

    private:
        std::size_t capacity_;
        std::list<std::pair<std::int32_t, std::int64_t>> cache_;
        std::unordered_map<std::int32_t, std::list<std::pair<std::int32_t, std::int64_t>>::iterator> cache_map_;
    };

    LRUCache::LRUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    void LRUCache::put(std::int32_t key, std::int64_t value) {
        auto it = cache_map_.find(key);
        if (it != cache_map_.end()) {
            it->second->second = value;
            cache_.splice(cache_.begin(), cache_, it->second);
            return;
        }

        if (cache_.size() == capacity_) {
            cache_map_.erase(cache_.back().first);
            cache_.pop_back();
        }

        cache_.emplace_front(key, value);
        cache_map_[key] = cache_.begin();
    }

    std::int64_t LRUCache::get(std::int32_t key) {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            throw std::out_of_range("Key not found");
        }

        cache_.splice(cache_.begin(), cache_, it->second);
        return it->second->second;
    }

    std::int64_t LRUCache::operator[](std::int32_t key) {
        return get(key);
    }

    bool LRUCache::contains(std::int32_t key) {
        return cache_map_.find(key) != cache_map_.end();
    }
}
