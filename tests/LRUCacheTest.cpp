#include <gtest/gtest.h>
#include <stdexcept>

import LRUCache;

TEST(LRUCacheTest, Constructor) {
    cache::LRUCache cache(5);

    cache.put(1, 100);

    EXPECT_EQ(cache.get(1), 100);
}

TEST(LRUCacheTest, ZeroCapacity) {
    EXPECT_THROW(cache::LRUCache cache(0), std::invalid_argument);
}

TEST(LRUCacheTest, Put) {
    cache::LRUCache cache(3);

    cache.put(1, 100);

    EXPECT_EQ(cache.get(1), 100);
}

TEST(LRUCacheTest, PutSeveral) {
    cache::LRUCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
}

TEST(LRUCacheTest, Get) {
    cache::LRUCache cache(3);

    cache.put(1, 100);

    EXPECT_EQ(cache.get(1), 100);
}

TEST(LRUCacheTest, GetException) {
    cache::LRUCache cache(3);

    EXPECT_THROW(cache.get(42), std::out_of_range);
}

TEST(LRUCacheTest, UpdateValue) {
    cache::LRUCache cache(3);

    cache.put(1, 100);
    cache.put(1, 500);

    EXPECT_EQ(cache.get(1), 500);
}

TEST(LRUCacheTest, Eviction) {
    cache::LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_THROW(cache.get(1), std::out_of_range);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
}

TEST(LRUCacheTest, GetChangesOrder) {
    cache::LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    cache.get(1);
    cache.put(3, 300);

    EXPECT_THROW(cache.get(2), std::out_of_range);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(3), 300);
}

TEST(LRUCacheTest, UpdateChangesOrder) {
    cache::LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    cache.put(1, 500);
    cache.put(3, 300);

    EXPECT_THROW(cache.get(2), std::out_of_range);
    EXPECT_EQ(cache.get(1), 500);
    EXPECT_EQ(cache.get(3), 300);
}

TEST(LRUCacheTest, MultipleEvictions) {
    cache::LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_THROW(cache.get(1), std::out_of_range);

    cache.put(4, 400);

    EXPECT_THROW(cache.get(2), std::out_of_range);
    EXPECT_EQ(cache.get(3), 300);
    EXPECT_EQ(cache.get(4), 400);
}

TEST(LRUCacheTest, Operator) {
    cache::LRUCache cache(3);

    cache.put(1, 100);

    EXPECT_EQ(cache[1], 100);
}

TEST(LRUCacheTest, OperatorException) {
    cache::LRUCache cache(3);

    EXPECT_THROW(cache[42], std::out_of_range);
}

TEST(LRUCacheTest, NegativeTTL) {
    EXPECT_THROW(
        cache::LRUCache cache(5, std::chrono::seconds(-1)),
        std::invalid_argument
    );
}

TEST(LRUCacheTest, TTLDisabled) {
    cache::LRUCache cache(5);

    cache.put(1, 100);

    EXPECT_TRUE(cache.contains(1));
    EXPECT_EQ(cache.get(1), 100);
}

TEST(LRUCacheTest, TTLNotExpired) {
    cache::LRUCache cache(5, std::chrono::seconds(10));

    cache.put(1, 100);

    EXPECT_TRUE(cache.contains(1));
    EXPECT_EQ(cache.get(1), 100);
}

TEST(LRUCacheTest, UpdateResetsTTL) {
    cache::LRUCache cache(5, std::chrono::seconds(3));

    cache.put(1, 100);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    cache.put(1, 500);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_TRUE(cache.contains(1));
    EXPECT_EQ(cache.get(1), 500);
}

TEST(LRUCacheTest, ExpiredElementCanBeInsertedAgain) {
    cache::LRUCache cache(1, std::chrono::seconds(1));

    cache.put(1, 100);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_FALSE(cache.contains(1));

    cache.put(2, 200);

    EXPECT_EQ(cache.get(2), 200);
}
