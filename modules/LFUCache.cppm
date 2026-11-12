module;

#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

export module LFUCache;

import ICache;

export  namespace cache {
    class LFUCache : public ICache {
    public:
        LFUCache(std::size_t capacity);

        void put(std::int32_t key, std::int64_t value) override;
        std::int64_t  get(std::int32_t key) override;
        std::int64_t operator[](std::int32_t key) override;
        bool contains(std::int32_t key) override;

    private:
        std::size_t capacity_;
        std::size_t access_counter_ = 0;

        std::unordered_map<std::int32_t, std::int64_t> values_;
        std::unordered_map<std::int32_t, std::size_t> frequencies_;
        std::unordered_map<std::int32_t, std::size_t> last_used_;

    };

    LFUCache::LFUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    void LFUCache::put(std::int32_t key, std::int64_t value) {
        auto it = values_.find(key);
        if (it != values_.end()) {
            it->second = value;

            ++frequencies_[key];
            last_used_[key] = ++access_counter_;

            return;
        }

        if (values_.size() == capacity_) {
            auto min_key = std::min_element(
                frequencies_.begin(),
                frequencies_.end(),
                [this](const auto& a, const auto& b) {
                    if (a.second != b.second) {
                        return a.second < b.second;
                    }
                    return last_used_[a.first] < last_used_[b.first];
                }
            )->first;

            values_.erase(min_key);
            frequencies_.erase(min_key);
            last_used_.erase(min_key);
        }

        values_[key] = value;
        frequencies_[key] = 1;
        last_used_[key] = ++access_counter_;
    }

    std::int64_t LFUCache::get(std::int32_t key) {
        auto it = values_.find(key);
        if (it == values_.end()) {
            throw std::out_of_range("Key not found");
        }

        ++frequencies_[key];
        last_used_[key] = ++access_counter_;

        return it->second;
    }

    std::int64_t LFUCache::operator[](std::int32_t key) {
        return get(key);
    }

    bool LFUCache::contains(std::int32_t key) {
        return values_.find(key) != values_.end();
    }
}
