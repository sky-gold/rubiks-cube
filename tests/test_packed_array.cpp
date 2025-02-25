#include <gtest/gtest.h>

#include "utils/packed_array.h"
#include <filesystem>

TEST(PackedArrayTest, ConstructorAndSize) {
    utils::PackedArray arr(10, 4);
    EXPECT_EQ(arr.Size(), 10);
}

TEST(PackedArrayTest, SetAndGet4Bits) {
    utils::PackedArray arr(10, 4);

    arr.Set(0, 5);
    EXPECT_EQ(arr.Get(0), 5);

    arr.Set(1, 15);
    EXPECT_EQ(arr.Get(1), 15);

    arr.Set(9, 7);
    EXPECT_EQ(arr.Get(9), 7);

    EXPECT_THROW(arr.Set(10, 5), std::out_of_range);
    EXPECT_THROW(arr.Get(10), std::out_of_range);
}

TEST(PackedArrayTest, SetAndGet8Bits) {
    utils::PackedArray arr(10, 8);

    arr.Set(0, 255);
    EXPECT_EQ(arr.Get(0), 255);

    arr.Set(1, 128);
    EXPECT_EQ(arr.Get(1), 128);

    arr.Set(9, 42);
    EXPECT_EQ(arr.Get(9), 42);
}

TEST(PackedArrayTest, SetAndGet2Bits) {
    utils::PackedArray arr(10, 2);

    arr.Set(0, 3);
    EXPECT_EQ(arr.Get(0), 3);

    arr.Set(1, 1);
    EXPECT_EQ(arr.Get(1), 1);

    arr.Set(9, 2);
    EXPECT_EQ(arr.Get(9), 2);
}

TEST(PackedArrayTest, WriteAndReadFile) {
    const std::string filename = "test_packed_array.bin";

    utils::PackedArray arr1(10, 4);
    arr1.Set(0, 5);
    arr1.Set(1, 15);
    arr1.Set(9, 7);

    arr1.WriteToFile(filename);

    utils::PackedArray arr2(10, 4);
    arr2.ReadFromFile(filename);

    EXPECT_EQ(arr2.Get(0), 5);
    EXPECT_EQ(arr2.Get(1), 15);
    EXPECT_EQ(arr2.Get(9), 7);

    std::filesystem::remove(filename);
}

TEST(PackedArrayTest, MoveSemantics) {
    utils::PackedArray arr1(10, 4);
    arr1.Set(0, 5);
    arr1.Set(1, 15);

    utils::PackedArray arr2 = std::move(arr1);

    EXPECT_EQ(arr2.Get(0), 5);
    EXPECT_EQ(arr2.Get(1), 15);
    EXPECT_EQ(arr2.Size(), 10);

    EXPECT_EQ(arr1.Size(), 0);
}