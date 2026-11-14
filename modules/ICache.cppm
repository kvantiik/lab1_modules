module;

#include <cstdint>

export module ICache;

export namespace cache {
    class ICache {
    public:
        virtual void put(std::int32_t key, std::int64_t value) = 0;

        virtual std::int64_t get(std::int32_t key) = 0;

        virtual std::int64_t operator[](std::int32_t key) = 0;

        virtual bool contains(std::int32_t key) = 0;

        virtual ~ICache() = default;
    };
}
