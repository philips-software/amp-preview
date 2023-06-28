#ifndef PREVIEW_BITMAP_HPP
#define PREVIEW_BITMAP_HPP

#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Colour.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace infra
{
    struct Bitmap
    {
        template<int32_t width, int32_t height, PixelFormat pixelFormat>
        struct WithStorage;
        template<int32_t width, int32_t height>
        using Rgb565 = WithStorage<width, height, PixelFormat::rgb565>;
        template<int32_t width, int32_t height>
        using Rgb888 = WithStorage<width, height, PixelFormat::rgb888>;
        template<int32_t width, int32_t height>
        using BlackAndWhite = WithStorage<width, height, PixelFormat::blackandwhite>;

        Bitmap(infra::ByteRange buffer, infra::Vector size, PixelFormat pixelFormat);

        const uint8_t* BufferAddress(infra::Point position) const;
        uint8_t* BufferAddress(infra::Point position);
        void SetBlackAndWhitePixel(infra::Point position, bool pixel);
        bool BlackAndWhitePixel(infra::Point position) const;
        uint32_t PixelColour(infra::Point position) const;

        infra::ByteRange buffer;
        infra::Vector size;
        PixelFormat pixelFormat;

        static constexpr uint32_t BufferSize(int32_t width, int32_t height, PixelFormat pixelFormat);
        static constexpr uint32_t BufferSizeBlackAndWhite(int32_t width, int32_t height);
        static uint32_t BufferSize(infra::Vector size, PixelFormat pixelFormat);
        uint32_t BufferSize();

        bool operator==(const Bitmap& other) const;
        void ConvertToBlackAndWhiteFromRgb565(Bitmap& colorBitmap);
        void ConvertToBlackAndWhiteFromRgb888(Bitmap& colorBitmap);
    };

    template<int32_t width_, int32_t height_, PixelFormat pixelFormat_>
    struct Bitmap::WithStorage
        : Bitmap
    {
    public:
        WithStorage();

    private:
        std::array<uint8_t, BufferSize(width_, height_, pixelFormat_)> storage;
    };

    // Implementation

    template<int32_t width_, int32_t height_, PixelFormat pixelFormat_>
    Bitmap::WithStorage<width_, height_, pixelFormat_>::WithStorage()
        : Bitmap(storage, infra::Vector(width_, height_), pixelFormat_)
    {}

    inline constexpr uint32_t Bitmap::BufferSize(int32_t width, int32_t height, PixelFormat pixelFormat)
    {
        return pixelFormat == PixelFormat::blackandwhite ? BufferSizeBlackAndWhite(width, height) : static_cast<std::size_t>(width * height) * PixelSize(pixelFormat);
    }

    inline constexpr uint32_t Bitmap::BufferSizeBlackAndWhite(int32_t width, int32_t height)
    {
        return static_cast<std::size_t>((width * height) / 8 + (((width * height) % 8) == 0 ? 0 : 1));
    }
}

#endif
