#include "preview/interfaces/Font.hpp"

namespace infra
{
    Glyph::Glyph(const uint8_t* bitmaps, const ConstructionInfo& info)
        : buffer(&bitmaps[info.bitmapOffset], &bitmaps[info.bitmapOffset] + (info.width * info.height + 7) / 8)
        , size(info.width, info.height)
        , cursorOffset(info.xOffset, info.yOffset)
        , advanceCursor(info.xAdvance)
    {}

    Font::Font(infra::MemoryRange<const Glyph> glyphs, char begin, char end, int16_t newlineDistance, int16_t cursorToTop)
        : begin(begin)
        , end(end)
        , glyphs(glyphs)
        , newlineDistance(newlineDistance)
        , cursorToTop(cursorToTop)
    {}

    uint16_t Font::Width(infra::BoundedConstString text) const
    {
        int16_t width = 0;

        for (auto c : text)
            width += glyphs[c - begin].advanceCursor;

        return width;
    }
}
