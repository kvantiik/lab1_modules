module;

#include <list>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>
#include <chrono>

export module LRUCache;

import ICache;

export namespace cache {
    class LRUCache : public ICache {
    public:
        LRUCache(std::size_t capacity, std::chrono::seconds ttl = std::chrono::seconds::zero());

        void put(std::int32_t key, std::int64_t value) override;

        std::int64_t get(std::int32_t key) override;

        std::int64_t operator[](std::int32_t key) override;

        bool contains(std::int32_t key) override;

    private:
        std::size_t capacity_;
        std::list<std::pair<std::int32_t, std::int64_t> > cache_;
        std::unordered_map<std::int32_t, std::list<std::pair<std::int32_t, std::int64_t> >::iterator> cache_map_;
        std::chrono::seconds ttl_;
        std::unordered_map<std::int32_t, std::chrono::steady_clock::time_point> validity_period_;

        void remove(std::int32_t key);
    };

    LRUCache::LRUCache(std::size_t capacity, std::chrono::seconds ttl) : capacity_(capacity), ttl_(ttl) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        if (ttl < std::chrono::seconds::zero()) {
            throw std::invalid_argument("TTL cannot be negative");
        }
    }

    void LRUCache::remove(std::int32_t key) {
        auto it = cache_map_.find(key);

        cache_.erase(it->second);
        cache_map_.erase(it);
        validity_period_.erase(key);
    }

    void LRUCache::put(std::int32_t key, std::int64_t value) {
        auto it = cache_map_.find(key);
        if (it != cache_map_.end()) {
            it->second->second = value;
            cache_.splice(cache_.begin(), cache_, it->second);
            if (ttl_ > std::chrono::seconds::zero()) {
                validity_period_[key] = std::chrono::steady_clock::now() + ttl_;
            }
            return;
        }

        if (cache_.size() == capacity_) {
            std::int32_t erase_key = cache_.back().first;
            cache_map_.erase(erase_key);
            validity_period_.erase(erase_key);
            cache_.pop_back();
        }

        cache_.emplace_front(key, value);
        cache_map_[key] = cache_.begin();

        if (ttl_ > std::chrono::seconds::zero()) {
            validity_period_[key] = std::chrono::steady_clock::now() + ttl_;
        }
    }

    std::int64_t LRUCache::get(std::int32_t key) {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            throw std::out_of_range("Key not found");
        }
        if (ttl_ > std::chrono::seconds::zero() && std::chrono::steady_clock::now() >= validity_period_.at(key)) {
            remove(key);

            throw std::out_of_range("Key expired");
        }

        cache_.splice(cache_.begin(), cache_, it->second);
        return it->second->second;
    }

    std::int64_t LRUCache::operator[](std::int32_t key) {
        return get(key);
    }

    bool LRUCache::contains(std::int32_t key) {
        auto it = cache_map_.find(key);

        if (it == cache_map_.end()) {
            return false;
        }

        if (ttl_ > std::chrono::seconds::zero() && std::chrono::steady_clock::now() >= validity_period_.at(key)) {
            remove(key);

            return false;
        }

        return true;
    }
}
