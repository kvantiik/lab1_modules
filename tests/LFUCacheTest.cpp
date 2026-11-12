#include <gtest/gtest.h>
#include <stdexcept>

import LFUCache;

TEST(LFUCacheTest, Constructor) {
    cache::LFUCache cache(5);

    cache.put(1, 100);
    EXPECT_EQ(100, cache.get(1));
}

TEST(LFUCacheTest, ZeroCapacity) {
    EXPECT_THROW(cache::LFUCache cache(0), std::invalid_argument);
}

TEST(LFUCacheTest, Put) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    EXPECT_EQ(100, cache.get(1));
}

TEST(LFUCacheTest, PutSeveral) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_EQ(100, cache.get(1));
    EXPECT_EQ(200, cache.get(2));
    EXPECT_EQ(300, cache.get(3));
}

TEST(LFUCacheTest, Get) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    EXPECT_EQ(100, cache.get(1));
}

TEST(LFUCacheTest, GetException) {
    cache::LFUCache cache(3);

    EXPECT_THROW(cache.get(69), std::out_of_range);
}

TEST(LFUCacheTest, PutUpdateValue) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    cache.put(1, 500);

    EXPECT_EQ(500, cache.get(1));
}

TEST(LFUCacheTest, PutIncreasesFrequency) {
    cache::LFUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    cache.put(1, 500);
    cache.put(3, 300);

    EXPECT_EQ(500, cache.get(1));
    EXPECT_EQ(300, cache.get(3));
    EXPECT_THROW(cache.get(2), std::out_of_range);
}

TEST(LFUCacheTest, GetIncreasesFrequency) {
    cache::LFUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    cache.get(1);
    cache.put(3, 300);

    EXPECT_EQ(100, cache.get(1));
    EXPECT_EQ(300, cache.get(3));
    EXPECT_THROW(cache.get(2), std::out_of_range);
}

TEST(LFUCacheTest, Eviction) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    cache.get(1);
    cache.get(1);
    cache.get(2);

    cache.put(4, 400);

    EXPECT_THROW(cache.get(3), std::out_of_range);

    EXPECT_EQ(100, cache.get(1));
    EXPECT_EQ(200, cache.get(2));
    EXPECT_EQ(400, cache.get(4));
}

TEST(LFUCacheTest, EvictionByLastUsed) {
    cache::LFUCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    cache.get(1);
    cache.get(2);
    cache.get(3);

    cache.put(4, 400);

    EXPECT_THROW(cache.get(1), std::out_of_range);

    EXPECT_EQ(200, cache.get(2));
    EXPECT_EQ(300, cache.get(3));
    EXPECT_EQ(400, cache.get(4));
}

TEST(LFUCacheTest, MultipleEvictions) {
    cache::LFUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_THROW(cache.get(1), std::out_of_range);

    cache.put(4, 400);

    EXPECT_THROW(cache.get(2), std::out_of_range);
    EXPECT_EQ(300, cache.get(3));
    EXPECT_EQ(400, cache.get(4));
}

TEST(LFUCacheTest, Operator) {
    cache::LFUCache cache(3);

    cache.put(1, 100);

    EXPECT_EQ(100, cache[1]);
}

TEST(LFUCacheTest, OperatorException) {
    cache::LFUCache cache(3);

    EXPECT_THROW(cache[42], std::out_of_range);
}

TEST(LFUCacheTest, OperatorIncreasesFrequency) {
    cache::LFUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    cache[1];
    cache.put(3, 300);

    EXPECT_EQ(100, cache.get(1));
    EXPECT_EQ(300, cache.get(3));
    EXPECT_THROW(cache.get(2), std::out_of_range);
}
