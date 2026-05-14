#include "preview/interfaces/Hub75Bitmap.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <bitset>

namespace infra
{
    Hub75Bitmap::Hub75Bitmap(infra::ByteRange buffer, infra::Vector size)
        : infra::Bitmap(size)
        , buffer(buffer)
        , blockSize(size.deltaY / 32)
    {
        assert(size.deltaY % 32 == 0);
        assert(buffer.size() == BufferSize(size.deltaX, size.deltaY));
    }

    void Hub75Bitmap::Clear()
    {
        for (auto i = 0; i != blockSize; ++i)
        {
            buffer[i + 0] = 0x00;
            buffer[i + 1] = 0x40;
            buffer[i + 2] = 0x80;
            buffer[i + 3] = 0xc0;
        }
    }

    infra::Colour Hub75Bitmap::PixelColour(infra::Point position) const
    {
        uint8_t mask = 7;
        uint8_t shift = 0;
        auto bufferPosition = position.y / 32 * blockSize + position.x;

        if (position.y % 32 >= 16)
        {
            mask <<= 3;
            shift = 3;
            bufferPosition /= 2;
        }

        auto bitColourPattern = (buffer[bufferPosition] & ~mask) >> shift;
        return CreateColour((bitColourPattern & 1) ? 255 : 0, (bitColourPattern & 2) ? 255 : 0, (bitColourPattern & 4) ? 255 : 0);
    }

    void Hub75Bitmap::DrawPixel(infra::Point position, infra::Colour colour)
    {
        auto red = RedFromColour(colour) != 0;
        auto green = GreenFromColour(colour) != 0;
        auto blue = BlueFromColour(colour) != 0;

        uint8_t bitColourPattern = (red ? 1 : 0) | (green ? 2 : 0) | (blue ? 4 : 0);
        uint8_t mask = 7;
        auto bufferPosition = position.y / 32 * blockSize + position.x;

        if (position.y % 32 >= 16)
        {
            bitColourPattern <<= 3;
            mask <<= 3;
            bufferPosition /= 2;
        }

        buffer[bufferPosition] = (buffer[bufferPosition] & ~mask) | bitColourPattern;
    }

    uint32_t Hub75Bitmap::BufferSize(infra::Vector size)
    {
        return BufferSize(size.deltaX, size.deltaY);
    }

    uint32_t Hub75Bitmap::BufferSize()
    {
        return buffer.size();
    }

    bool Hub75Bitmap::operator==(const Hub75Bitmap& other) const
    {
        return other.size == size && other.buffer == buffer;
    }
}
