#ifndef PREVIEW_HUB75_BITMAP_HPP
#define PREVIEW_HUB75_BITMAP_HPP

#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace infra
{
    struct Hub75Bitmap
        : public Bitmap
    {
        template<int32_t width, int32_t height>
        struct WithDimensions;

        Hub75Bitmap(infra::ByteRange buffer, infra::Vector size, infra::Vector panelSize);

        void Clear();

        infra::Colour PixelColour(infra::Point position) const override;
        void DrawPixel(infra::Point position, infra::Colour colour) override;

        infra::ByteRange buffer;
        infra::Vector panelSize;

    private:
        uint16_t blockSize;

    public:
        static constexpr uint32_t BufferSize(int32_t width, int32_t height);
        static uint32_t BufferSize(infra::Vector size);
        uint32_t BufferSize();

        bool operator==(const Hub75Bitmap& other) const;

    private:
        struct BitPosition
        {
            uint8_t patternShift;
            uint32_t bufferPosition;
        };

        BitPosition CalculatePosition(infra::Point position) const;
    };

    template<int32_t width_, int32_t height_>
    struct Hub75Bitmap::WithDimensions
        : Hub75Bitmap
    {
    public:
        WithDimensions(infra::Vector panelSize);

    private:
        std::array<uint8_t, BufferSize(width_, height_)> storage;
    };

    // Implementation

    template<int32_t width_, int32_t height_>
    Hub75Bitmap::WithDimensions<width_, height_>::WithDimensions(infra::Vector panelSize)
        : Hub75Bitmap(storage, infra::Vector(width_, height_), panelSize)
    {}

    constexpr uint32_t Hub75Bitmap::BufferSize(int32_t width, int32_t height)
    {
        // Each byte contains 2 times 3 bits for two pixels, plus clock
        // each byte is repeated with clock low and clock high
        return static_cast<std::size_t>(width * height);
    }
}

#endif
