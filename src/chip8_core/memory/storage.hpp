#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <functional>

class Storage
{
public:
    const std::array<std::uint8_t, 16>& read() const;
    void write(const std::vector<std::uint8_t>& newData);

    void setData(const std::array<std::uint8_t, 16>& newData);
    void setOnWriteCallback(std::function<void(const std::array<std::uint8_t, 16>&)> onWriteCallback);

private:
    std::array<std::uint8_t, 16> data{};
    std::function<void(const std::array<std::uint8_t, 16>&)> onWriteCallback;
};