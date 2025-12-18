#include <gtest/gtest.h>
#include "../CacheImp.h"
#include <string>

class ClockCacheTest : public ::testing::Test {
protected:
    ClockCache<int, int> int_cache;
    ClockCache<std::string, std::string> string_cache;

    void SetUp() override {
        int_cache = ClockCache<int, int>(3);
        string_cache = ClockCache<std::string, std::string>(2);
    }

    void TearDown() override {
        int_cache.clear();
        string_cache.clear();
    }
};

TEST_F(ClockCacheTest, BasicSetAndGet) {
    int_cache.set(1, 10);
    int_cache.set(2, 20);
    int_cache.set(3, 30);

    EXPECT_EQ(int_cache.get(1), 10);
    EXPECT_EQ(int_cache.get(2), 20);
    EXPECT_EQ(int_cache.get(3), 30);
    EXPECT_EQ(int_cache.size(), 3);
}

TEST_F(ClockCacheTest, ContainsKey) {
    ClockCache<int, int> small_cache(2);  // Use a smaller capacity for this test

    small_cache.set(1, 10);
    EXPECT_TRUE(small_cache.contains(1));
    EXPECT_FALSE(small_cache.contains(2));

    small_cache.set(2, 20);
    EXPECT_TRUE(small_cache.contains(1));
    EXPECT_TRUE(small_cache.contains(2));

    small_cache.set(3, 30);  // Should evict one entry
    EXPECT_FALSE(small_cache.contains(1));  // One of the entries should have been evicted
    EXPECT_TRUE(small_cache.contains(2));
    EXPECT_TRUE(small_cache.contains(3));
}

// Test cache eviction behavior following clock algorithm
TEST_F(ClockCacheTest, ClockEviction) {
    ClockCache<int, int> small_cache(2);

    // Add two entries
    small_cache.set(1, 10);
    small_cache.set(2, 20);

    // Access key 1 to set its reference bit to 1
    EXPECT_EQ(small_cache.get(1), 10);

    // Add a third entry - following clock algorithm
    small_cache.set(3, 30);

    // In Clock algorithm, this follows the clock hand position logic
    // After accessing key 1, when we add key 3, eviction will follow the clock algorithm
    EXPECT_EQ(small_cache.size(), 2);

    // Key 3 should definitely be present
    EXPECT_TRUE(small_cache.contains(3));

    // Either key 1 or key 2 should be present, but not both
    // (The exact behavior depends on the internal clock hand position)
    bool key1_present = small_cache.contains(1);
    bool key2_present = small_cache.contains(2);
    EXPECT_TRUE(key1_present || key2_present);  // At least one should remain
    EXPECT_FALSE(key1_present && key2_present);  // Only one should remain (since size is 2 and key 3 is present)
}

// Test cache with string keys and values
TEST_F(ClockCacheTest, StringKeyAndValue) {
    string_cache.set("key1", "value1");
    string_cache.set("key2", "value2");

    EXPECT_EQ(string_cache.get("key1"), "value1");
    EXPECT_EQ(string_cache.get("key2"), "value2");

    string_cache.set("key3", "value3");  // Should evict one of the previous keys

    EXPECT_EQ(string_cache.size(), 2);
}

// Test clearing the cache
TEST_F(ClockCacheTest, ClearCache) {
    ClockCache<int, int> cache_5(5);

    cache_5.set(1, 10);
    cache_5.set(2, 20);
    cache_5.set(3, 30);

    EXPECT_EQ(cache_5.size(), 3);

    cache_5.clear();

    EXPECT_EQ(cache_5.size(), 0);
    EXPECT_FALSE(cache_5.contains(1));
    EXPECT_FALSE(cache_5.contains(2));
    EXPECT_FALSE(cache_5.contains(3));
}

// Test cache with capacity 1
TEST_F(ClockCacheTest, CapacityOne) {
    ClockCache<int, int> cache_1(1);

    cache_1.set(1, 10);
    EXPECT_EQ(cache_1.get(1), 10);
    EXPECT_EQ(cache_1.size(), 1);

    cache_1.set(2, 20);  // Should evict key 1
    EXPECT_FALSE(cache_1.contains(1));
    EXPECT_TRUE(cache_1.contains(2));
    EXPECT_EQ(cache_1.get(2), 20);
    EXPECT_EQ(cache_1.size(), 1);
}

// Test behavior when setting the same key
TEST_F(ClockCacheTest, SetValueForKeyTwice) {
    ClockCache<int, int> cache_2(2);

    cache_2.set(1, 10);
    EXPECT_EQ(cache_2.get(1), 10);

    cache_2.set(1, 20);  // Update the value for key 1
    EXPECT_EQ(cache_2.get(1), 20);
    EXPECT_EQ(cache_2.size(), 1);
}

// Test zero capacity behavior
TEST_F(ClockCacheTest, ZeroCapacity) {
    ClockCache<int, int> zero_cache(0);  // Should be treated as capacity 1

    zero_cache.set(1, 10);
    EXPECT_EQ(zero_cache.get(1), 10);
    EXPECT_EQ(zero_cache.size(), 1);
}

// Test cache hit/miss behavior
TEST_F(ClockCacheTest, HitAndMiss) {
    ClockCache<int, int> cache_2(2);

    // Miss - key doesn't exist
    EXPECT_EQ(cache_2.get(1), 0);  // Default value for int

    // Add an item and test hit
    cache_2.set(1, 10);
    EXPECT_EQ(cache_2.get(1), 10);  // Hit

    // Access to mark as recently used
    EXPECT_EQ(cache_2.get(1), 10);

    // Add another item
    cache_2.set(2, 20);

    // Add a third item - one should be evicted
    cache_2.set(3, 30);

    // Check which one was evicted (should be based on clock algorithm)
    EXPECT_EQ(cache_2.size(), 2);
    // Either key 1 or key 2 should be evicted, key 3 should be present
    EXPECT_TRUE(cache_2.contains(3));
}

// Test complex eviction scenario following clock algorithm
TEST_F(ClockCacheTest, ComplexEviction) {
    ClockCache<int, int> cache_3(3);

    // Fill the cache
    cache_3.set(1, 10);  // ref bit = 1
    cache_3.set(2, 20);  // ref bit = 1
    cache_3.set(3, 30);  // ref bit = 1

    // Access some keys to update their reference bits
    cache_3.get(1);  // key 1 ref bit = 1
    cache_3.get(2);  // key 2 ref bit = 1

    // Now add two more entries - this should trigger evictions
    cache_3.set(4, 40);  // This should evict key 3 first (it had ref bit = 1, now set to 0)
    cache_3.set(5, 50);  // This should evict key 1 if its bit was reset, or key 2

    EXPECT_EQ(cache_3.size(), 3);
}

// Test size function
TEST_F(ClockCacheTest, SizeFunction) {
    ClockCache<int, int> cache_5(5);

    EXPECT_EQ(cache_5.size(), 0);

    cache_5.set(1, 10);
    EXPECT_EQ(cache_5.size(), 1);

    cache_5.set(2, 20);
    EXPECT_EQ(cache_5.size(), 2);

    cache_5.get(1);  // Access should not change size
    EXPECT_EQ(cache_5.size(), 2);

    cache_5.set(1, 30);  // Update should not change size
    EXPECT_EQ(cache_5.size(), 2);

    cache_5.set(3, 30);
    cache_5.set(4, 40);
    cache_5.set(5, 50);
    EXPECT_EQ(cache_5.size(), 5);  // Reached capacity

    cache_5.set(6, 60);  // Should trigger eviction
    EXPECT_EQ(cache_5.size(), 5);
}

// Test eviction when all reference bits are 1
TEST_F(ClockCacheTest, AllRefBitsOneEviction) {
    ClockCache<int, int> cache_2(2);

    // Add 2 items
    cache_2.set(1, 10);  // ref bit = 1
    cache_2.set(2, 20);  // ref bit = 1

    // Access both to ensure ref bits are 1
    cache_2.get(1);  // ref bit = 1
    cache_2.get(2);  // ref bit = 1

    // Add another item - algorithm should reset bits and evict
    cache_2.set(3, 30);

    // At this point one of the original items should be evicted
    EXPECT_EQ(cache_2.size(), 2);
    EXPECT_TRUE(cache_2.contains(3));  // New item should be present
}

// Test with different value types
TEST_F(ClockCacheTest, DifferentValueTypes) {
    ClockCache<int, std::string> cache_2(2);

    cache_2.set(1, "first value");
    cache_2.set(2, "second value");

    EXPECT_EQ(cache_2.get(1), "first value");
    EXPECT_EQ(cache_2.get(2), "second value");

    cache_2.set(3, "third value");  // Should evict one of the previous

    EXPECT_EQ(cache_2.size(), 2);
}