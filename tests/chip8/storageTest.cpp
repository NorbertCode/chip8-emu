#include <array>
#include <gtest/gtest.h>
#include <vector>
#include "memory/storage.hpp"

using namespace chip8::core;

TEST(StorageTest, Write_LessThanMaxElements_CallbackIsCalledWithCorrectData)
{
    std::array<std::uint8_t, 16> callbackData{};
    Storage storage;
    storage.setOnWriteCallback([&callbackData](std::span<const std::uint8_t, 16> data) {
        std::ranges::copy(data, callbackData.begin());
    });
    std::array<std::uint8_t, 16> expectedData = { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0 };

    std::vector<std::uint8_t> newData = { 1, 2, 3, 4, 5, 6, 7, 8 };
    storage.write(newData);

    EXPECT_EQ(callbackData, expectedData);
}

TEST(StorageTest, Write_MoreThanMaxElements_CallbackIsCalledWithCorrectData)
{
    std::array<std::uint8_t, 16> callbackData{};
    Storage storage;
    storage.setOnWriteCallback([&callbackData](std::span<const std::uint8_t, 16> data) {
        std::ranges::copy(data, callbackData.begin());
    });
    std::array<std::uint8_t, 16> expectedData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    std::vector<std::uint8_t> newData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
    storage.write(newData);

    EXPECT_EQ(callbackData, expectedData);
}

TEST(StorageTest, Write_ExistingData_OverwritesData)
{
    std::array<std::uint8_t, 16> callbackData{};
    Storage storage;
    storage.setOnWriteCallback([&callbackData](std::span<const std::uint8_t, 16> data) {
        std::ranges::copy(data, callbackData.begin());
    });
    std::array<std::uint8_t, 16> initialData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    std::array<std::uint8_t, 16> expectedData = { 17, 18, 19, 20, 21, 22, 23, 24, 9, 10, 11, 12, 13, 14, 15, 16 };

    std::vector<std::uint8_t> newData = { 17, 18, 19, 20, 21, 22, 23, 24 };
    storage.setData(initialData);
    storage.write(newData);

    EXPECT_EQ(callbackData, expectedData);
}

TEST(StorageTest, SetData_CallbackIsNotCalled)
{
    bool callbackCalled = false;
    Storage storage;
    storage.setOnWriteCallback([&callbackCalled](std::span<const std::uint8_t, 16>) {
        callbackCalled = true;
    });
    std::array<std::uint8_t, 16> newData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    storage.setData(newData);

    EXPECT_FALSE(callbackCalled);
}