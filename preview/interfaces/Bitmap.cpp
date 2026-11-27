#include "preview/interfaces/Bitmap.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <bitset>

namespace infra
{
    Bitmap::Bitmap(infra::Vector size)
        : size(size)
    {}

    SimpleBitmap::SimpleBitmap(infra::ByteRange buffer, infra::Vector size, PixelFormat pixelFormat)
        : Bitmap(size)
        , buffer(buffer)
        , pixelFormat(pixelFormat)
    {
        isSimple = true;
        assert(buffer.size() == BufferSize(size.deltaX, size.deltaY, pixelFormat));
    }

    void SimpleBitmap::Clear()
    {
        std::fill(buffer.begin(), buffer.end(), 0);
    }

    const uint8_t* SimpleBitmap::BufferAddress(infra::Point position) const
    {
        assert(pixelFormat != PixelFormat::blackandwhite);

        return buffer.begin() + PixelSize(pixelFormat) * (position.y * size.deltaX + position.x);
    }

    uint8_t* SimpleBitmap::BufferAddress(infra::Point position)
    {
        assert(pixelFormat != PixelFormat::blackandwhite);

        return buffer.begin() + PixelSize(pixelFormat) * (position.y * size.deltaX + position.x);
    }

    void SimpleBitmap::SetBlackAndWhitePixel(infra::Point position, bool pixel)
    {
        assert(pixelFormat == PixelFormat::blackandwhite);

        auto bitIndex = position.y * size.deltaX + position.x;
        infra::ReplaceBit(buffer[bitIndex / 8], pixel, bitIndex & 7);
    }

    bool SimpleBitmap::BlackAndWhitePixel(infra::Point position) const
    {
        assert(pixelFormat == PixelFormat::blackandwhite);

        auto bitIndex = position.y * size.deltaX + position.x;
        return (buffer[bitIndex / 8] & (1 << (bitIndex % 8))) != 0;
    }

    infra::Colour SimpleBitmap::PixelColour(infra::Point position) const
    {
        return infra::ConvertToRgb888(RawPixelColour(position), pixelFormat);
    }

    uint32_t SimpleBitmap::RawPixelColour(infra::Point position) const
    {
        auto pixel = BufferAddress(position);
        uint32_t colour = 0;
        switch (infra::PixelSize(pixelFormat))
        {
            case 1:
                colour = pixel[0];
                break;
            case 2:
                colour = pixel[0] + (pixel[1] << 8);
                break;
            case 3:
                colour = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);
                break;
            default:
                std::abort();
        }

        return colour;
    }

    void SimpleBitmap::DrawPixel(infra::Point position, infra::Colour colour)
    {
        switch (pixelFormat)
        {
            case infra::PixelFormat::rgb565:
            {
                auto convertedColour = infra::ConvertRgb888ToRgb565(colour);
                std::memcpy(BufferAddress(position), &convertedColour, 2);
                break;
            }
            case infra::PixelFormat::rgb888:
                std::memcpy(BufferAddress(position), &colour, 3);
                break;
            case infra::PixelFormat::blackandwhite:
                SetBlackAndWhitePixel(position, infra::ConvertRgb888ToBlackAndWhite(colour));
                break;
            default:
                std::abort();
        }
    }

    uint32_t SimpleBitmap::BufferSize(infra::Vector size, PixelFormat pixelFormat)
    {
        return BufferSize(size.deltaX, size.deltaY, pixelFormat);
    }

    uint32_t SimpleBitmap::BufferSize()
    {
        return buffer.size();
    }

    void SimpleBitmap::ConvertToBlackAndWhiteFromRgb565(SimpleBitmap& colorBitmap)
    {
        if (pixelFormat != PixelFormat::blackandwhite || colorBitmap.pixelFormat != PixelFormat::rgb565)
            std::abort();

        if (size != colorBitmap.size)
            std::abort();

        for (int i = 0; i < colorBitmap.size.deltaX; i++)
            for (int j = 0; j < colorBitmap.size.deltaY; j++)
            {
                auto pixelColour = static_cast<uint32_t>(*reinterpret_cast<uint16_t*>(colorBitmap.BufferAddress(infra::Point(i, j))));

                uint32_t bitIndex = j * size.deltaX + i;

                std::bitset<8> byte = buffer[bitIndex / 8];
                byte[bitIndex % 8] = ConvertRgb565ToBlackAndWhite(pixelColour);
                buffer[bitIndex / 8] = static_cast<uint8_t>(byte.to_ulong());
            }
    }

    void SimpleBitmap::ConvertToBlackAndWhiteFromRgb888(SimpleBitmap& colorBitmap)
    {
        if (pixelFormat != PixelFormat::blackandwhite || colorBitmap.pixelFormat != PixelFormat::rgb888)
            std::abort();

        if (size != colorBitmap.size)
            std::abort();

        for (int i = 0; i < colorBitmap.size.deltaX; i++)
            for (int j = 0; j < colorBitmap.size.deltaY; j++)
            {
                uint32_t pixelColour = *reinterpret_cast<uint32_t*>(colorBitmap.BufferAddress(infra::Point(i, j)));
                pixelColour &= 0x00ffffff;

                uint32_t bitIndex = j * size.deltaX + i;

                std::bitset<8> byte = buffer[bitIndex / 8];
                byte[bitIndex % 8] = ConvertRgb888ToBlackAndWhite(static_cast<Colour>(pixelColour));
                buffer[bitIndex / 8] = static_cast<uint8_t>(byte.to_ulong());
            }
    }

    bool SimpleBitmap::operator==(const SimpleBitmap& other) const
    {
        return other.size == size && other.buffer == buffer && other.pixelFormat == pixelFormat;
    }
}
