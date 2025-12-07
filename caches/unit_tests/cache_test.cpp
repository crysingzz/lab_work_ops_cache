#include <gtest/gtest.h>
#include "../CacheImp.h"

class CacheTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(CacheTest, PutAndGet) {
    ExampleCache<int, int> cache;
    EXPECT_EQ(cache.size(), 1);
}

