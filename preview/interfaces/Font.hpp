#ifndef PREVIEW_FONT_HPP
#define PREVIEW_FONT_HPP

#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace infra
{
    enum class FontPixelFormat
    {
        a1,
        a4,
        a8
    };

    class Glyph
    {
    public:
        struct ConstructionInfo
        {
            uint16_t bitmapOffset;
            uint8_t width;
            uint8_t height;
            uint8_t xAdvance;
            int8_t xOffset;
            int8_t yOffset;
        };

        Glyph() = default;
        Glyph(const uint8_t* bitmaps, const ConstructionInfo& info);

        infra::ConstByteRange buffer;
        infra::Vector size;
        infra::Vector cursorOffset;
        int32_t advanceCursor;
    };

    class Font
    {
    public:
        Font() = default;
        Font(infra::MemoryRange<const Glyph> glyphs, char begin, char end, int16_t newlineDistance, int16_t cursorToTop);

        char begin;
        char end;
        infra::MemoryRange<const Glyph> glyphs;

        uint16_t Width(infra::BoundedConstString text) const;

        FontPixelFormat pixelFormat = FontPixelFormat::a1;
        int16_t newlineDistance;
        int16_t cursorToTop;
    };
}

#endif
