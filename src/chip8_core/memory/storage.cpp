#include "storage.hpp"
#include <algorithm>

const std::array<std::uint8_t, 16>& Storage::read() const
{
    return data;
}

void Storage::write(const std::vector<uint8_t>& newData)
{
    size_t size = std::min<size_t>(newData.size(), data.size());

    std::copy(newData.begin(), newData.begin() + size, data.begin());

    if (onWriteCallback)
        onWriteCallback(data);
}

void Storage::setData(const std::array<std::uint8_t, 16>& newData)
{
    data = newData;
}

void Storage::setOnWriteCallback(std::function<void(const std::array<std::uint8_t, 16>&)> onWriteCallback)
{
    this->onWriteCallback = std::move(onWriteCallback);
}