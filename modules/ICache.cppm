export module ICache;

export class ICache {
public:
    virtual void put(int key, long long value) = 0;
    virtual long long get(int key) = 0;
    virtual long long operator[](int key) = 0;

    virtual ~ICache() = default;
};