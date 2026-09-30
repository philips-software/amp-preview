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
        struct WithStorage;

        Hub75Bitmap(infra::ByteRange buffer, infra::Vector size);

        void Clear();

        infra::Colour PixelColour(infra::Point position) const override;
        void DrawPixel(infra::Point position, infra::Colour colour) override;

        infra::ByteRange buffer;
        infra::Vector size;
        uint16_t blockSize;

        static constexpr uint32_t BufferSize(int32_t width, int32_t height);
        static uint32_t BufferSize(infra::Vector size);
        uint32_t BufferSize();

        bool operator==(const Hub75Bitmap& other) const;
    };

    template<int32_t width_, int32_t height_>
    struct Hub75Bitmap::WithStorage
        : Hub75Bitmap
    {
    public:
        WithStorage();

    private:
        std::array<uint8_t, BufferSize(width_, height_)> storage;
    };

    // Implementation

    template<int32_t width_, int32_t height_>
    Hub75Bitmap::WithStorage<width_, height_>::WithStorage()
        : Hub75Bitmap(storage, infra::Vector(width_, height_))
    {}

    constexpr uint32_t Hub75Bitmap::BufferSize(int32_t width, int32_t height)
    {
        // Each byte contains 2 times 3 bits for two pixels, plus clock
        // each byte is repeated with clock low and clock high
        return static_cast<std::size_t>(width * height);
    }
}

#endif
