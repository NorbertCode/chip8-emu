#pragma once
#include <array>
#include <span>
#include <cstdint>
#include <functional>

namespace chip8::core
{
    class Storage
    {
    public:
        std::span<const std::uint8_t, 16> read() const;
        void write(std::span<const std::uint8_t> newData);

        void setData(std::span<const std::uint8_t, 16> newData);
        void setOnWriteCallback(std::function<void(std::span<const std::uint8_t, 16>)> newOnWriteCallback);

    private:
        std::array<std::uint8_t, 16> data{};
        std::function<void(std::span<const std::uint8_t, 16>)> onWriteCallback;
    };
}