#include "storage.hpp"
#include <algorithm>
#include <cstddef>

namespace chip8::core
{
    std::span<const std::uint8_t, 16> Storage::read() const
    {
        return std::span<const std::uint8_t, 16>(data);
    }

    void Storage::write(const std::span<const std::uint8_t> newData)
    {
        std::ptrdiff_t size = static_cast<std::ptrdiff_t>(std::min<size_t>(newData.size(), data.size()));

        std::ranges::copy(newData.begin(), newData.begin() + size, data.begin());

        if (onWriteCallback)
            onWriteCallback(std::span<const std::uint8_t, 16>(data));
    }

    void Storage::setData(std::span<const std::uint8_t, 16> newData)
    {
        std::ranges::copy(newData, data.begin());
    }

    void Storage::setOnWriteCallback(std::function<void(std::span<const std::uint8_t, 16>)> onWriteCallback)
    {
        this->onWriteCallback = std::move(onWriteCallback);
    }
}