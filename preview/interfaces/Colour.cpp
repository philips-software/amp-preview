#include "preview/interfaces/Colour.hpp"

namespace infra
{
    Colour CreateColour(uint8_t red, uint8_t green, uint8_t blue)
    {
        return static_cast<Colour>((static_cast<uint32_t>(red) << 16) | (static_cast<uint32_t>(green) << 8) | blue);
    }

    uint8_t RedFromColour(Colour c)
    {
        return static_cast<uint8_t>(static_cast<uint32_t>(c) >> 16);
    }

    uint8_t GreenFromColour(Colour c)
    {
        return static_cast<uint8_t>(static_cast<uint32_t>(c) >> 8);
    }

    uint8_t BlueFromColour(Colour c)
    {
        return static_cast<uint8_t>(c);
    }

    Colour AverageColour(Colour x, Colour y)
    {
        return CreateColour((RedFromColour(x) + RedFromColour(y)) / 2, (GreenFromColour(x) + GreenFromColour(y)) / 2, (BlueFromColour(x) + BlueFromColour(y)) / 2);
    }

    Colour BetweenColour(Colour x, Colour y, uint16_t proportion)
    {
        return CreateColour(
            ((256 - proportion) * RedFromColour(x) + proportion * RedFromColour(y)) / 256,
            ((256 - proportion) * GreenFromColour(x) + proportion * GreenFromColour(y)) / 256,
            ((256 - proportion) * BlueFromColour(x) + proportion * BlueFromColour(y)) / 256);
    }

    Colour ConvertRgb565ToRgb888(uint32_t colour)
    {
        // Input colour: RRRRR GGGGGG BBBBB
        // Output colour: RRRRR000 GGGGGG00 BBBBB000
        return static_cast<Colour>(((colour & 0xf800) << 8) | ((colour & 0x07e0) << 5) | ((colour & 0x001f) << 3));
    }

    uint32_t ConvertRgb888ToRgb565(Colour colour)
    {
        // Input colour: RRRRR000 GGGGGG00 BBBBB000
        // Output colour: RRRRR GGGGGG BBBBB
        return ((static_cast<uint32_t>(colour) & 0xf80000) >> 8) | ((static_cast<uint32_t>(colour) & 0xfc00) >> 5) | ((static_cast<uint32_t>(colour) & 0x00F8) >> 3);
    }

    uint32_t ConvertRgb888To(Colour colour, PixelFormat format)
    {
        switch (format)
        {
            case PixelFormat::rgb888:
                return static_cast<uint32_t>(colour);
            case PixelFormat::rgb565:
                return ConvertRgb888ToRgb565(colour);
            case PixelFormat::blackandwhite:
                return colour == infra::Colour::white ? 1 : 0;
            default:
                std::abort();
        }
    }

    Colour ConvertToRgb888(uint32_t colour, PixelFormat format)
    {
        switch (format)
        {
            case PixelFormat::rgb888:
                return static_cast<Colour>(colour);
            case PixelFormat::rgb565:
                return ConvertRgb565ToRgb888(colour);
            case PixelFormat::blackandwhite:
                return colour == 0 ? infra::Colour::black : infra::Colour::white;
            default:
                std::abort();
        }
    }

    bool ConvertRgb565ToBlackAndWhite(uint32_t colour)
    {
        return colour == ConvertRgb888ToRgb565(Colour::white);
    }

    bool ConvertRgb888ToBlackAndWhite(Colour colour)
    {
        return colour == Colour::white;
    }
}
