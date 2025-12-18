#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include "../KwayMerge.h"

class MergeTest : public ::testing::Test {
protected:
    std::vector<int> test_array;
    std::vector<float> float_array;
    std::vector<std::string> string_array;
    std::vector<int> empty_array;
    std::vector<int> zeroK_array;
    std::vector<int> single_element_array;
    std::vector<int> sorted_array;
    std::vector<int> reverse_sorted_array;
    std::vector<int> duplicate_elements_array;
    std::vector<int> k_greater_than_elements_array;
    std::vector<int> block_size_larger_array;
    std::vector<int> block_size_one_array;
    std::vector<int> negative_numbers_array;
    std::vector<int> large_block_size_array;
    std::vector<int> uneven_distribution_array;
    std::vector<int> exactly_divisible_array;

    void SetUp() override {
        test_array = {64, 34, 25, 12, 22, 11, 90, 5};
        float_array = {3.5f, 1.2f, 4.8f, 2.1f, 6.7f, 0.5f};
        string_array = {"banana", "apple", "cherry", "date", "elderberry"};
        empty_array = {};
        zeroK_array = {5, 2, 8, 1};
        single_element_array = {42};
        sorted_array = {1, 2, 3, 4, 5, 6, 7, 8};
        reverse_sorted_array = {8, 7, 6, 5, 4, 3, 2, 1};
        duplicate_elements_array = {5, 2, 8, 2, 9, 1, 5, 4};
        k_greater_than_elements_array = {3, 1, 4};
        block_size_larger_array = {5, 2, 8, 1};
        block_size_one_array = {4, 1, 3, 2};
        negative_numbers_array = {5, -2, -8, 1, 9, -1};
        large_block_size_array = {64, 34, 25, 12, 22, 11, 90, 5};
        uneven_distribution_array = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        exactly_divisible_array = {8, 7, 6, 5, 4, 3, 2, 1};
    }

    void TearDown() override {
        test_array.clear();
        float_array.clear();
        string_array.clear();
        empty_array.clear();
        zeroK_array.clear();
        single_element_array.clear();
        sorted_array.clear();
        reverse_sorted_array.clear();
        duplicate_elements_array.clear();
        k_greater_than_elements_array.clear();
        block_size_larger_array.clear();
        block_size_one_array.clear();
        negative_numbers_array.clear();
        large_block_size_array.clear();
        uneven_distribution_array.clear();
        exactly_divisible_array.clear();
    }
};

TEST_F(MergeTest, EmptyArray) {
    auto result = KwayMergeSort(empty_array);
    EXPECT_EQ(result.size(), 0);
}

TEST_F(MergeTest, ZeroK) {
    auto result = KwayMergeSort(zeroK_array, 0, 8);
    EXPECT_EQ(result.size(), 0);
}

TEST_F(MergeTest, SingleElement) {
    auto result = KwayMergeSort(single_element_array, 4, 8);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

TEST_F(MergeTest, AlreadySortedArray) {
    auto result = KwayMergeSort(sorted_array, 4, 4);
    EXPECT_TRUE(std::is_sorted(result.begin(), result.end()));
    EXPECT_EQ(result, sorted_array);
}

TEST_F(MergeTest, ReverseSortedArray) {
    auto result = KwayMergeSort(reverse_sorted_array, 4, 4);
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, RandomUnsortedArray) {
    std::vector<int> array = test_array;
    auto result = KwayMergeSort(array, 4, 4);
    std::sort(array.begin(), array.end());
    EXPECT_EQ(result, array);
}

TEST_F(MergeTest, DuplicateElements) {
    auto result = KwayMergeSort(duplicate_elements_array, 4, 4);
    std::vector<int> expected = duplicate_elements_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, KGreaterThanElements) {
    auto result = KwayMergeSort(k_greater_than_elements_array, 10, 8);
    std::vector<int> expected = k_greater_than_elements_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, BlockSizeLargerThanArray) {
    auto result = KwayMergeSort(block_size_larger_array, 4, 10);
    std::vector<int> expected = block_size_larger_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, BlockSizeOfOne) {
    auto result = KwayMergeSort(block_size_one_array, 4, 1);
    std::vector<int> expected = block_size_one_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, NegativeNumbers) {
    auto result = KwayMergeSort(negative_numbers_array, 3, 3);
    std::vector<int> expected = negative_numbers_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, StringElements) {
    std::vector<std::string> array = {"banana", "apple", "cherry", "date"};
    auto result = KwayMergeSort(array, 4, 2);
    std::sort(array.begin(), array.end());
    EXPECT_EQ(result, array);
}

TEST_F(MergeTest, TestStringsWithSetUpData) {
    auto result = KwayMergeSort(string_array, 2, 3);
    std::vector<std::string> expected = string_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, LargeBlockSize) {
    auto result = KwayMergeSort(large_block_size_array, 1, 20);
    std::vector<int> expected = large_block_size_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, UnevenDistribution) {
    auto result = KwayMergeSort(uneven_distribution_array, 3, 4);
    std::vector<int> expected = uneven_distribution_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, ExactlyDivisible) {
    auto result = KwayMergeSort(exactly_divisible_array, 2, 4);
    std::vector<int> expected = exactly_divisible_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST_F(MergeTest, TestWithSetUpData) {
    std::vector<int> original = test_array;
    auto result = KwayMergeSort(test_array, 3, 3);
    std::sort(original.begin(), original.end());
    EXPECT_EQ(result, original);
}
TEST_F(MergeTest, FloatNumbers) {
    std::vector<float> array = {3.5f, 1.2f, 4.8f, 2.1f};
    auto result = KwayMergeSort(array, 4, 2);
    std::sort(array.begin(), array.end());
    EXPECT_EQ(result, array);
}

TEST_F(MergeTest, TestFloatsWithSetUpData) {
    auto result = KwayMergeSort(float_array, 2, 3);
    std::vector<float> expected = float_array;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

