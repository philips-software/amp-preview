#ifndef PREVIEW_HUB75_BITMAP_HPP
#define PREVIEW_HUB75_BITMAP_HPP

#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/Colour.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace infra
{
    struct Hub75Bitmap
    {
        template<int32_t width, int32_t height>
        struct WithStorage;

        Hub75Bitmap(infra::ByteRange buffer, infra::Vector size);

        void Clear();

        infra::ByteRange buffer;
        infra::Vector size;

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
        return static_cast<std::size_t>(width * height) / 2;
    }
}

#endif
