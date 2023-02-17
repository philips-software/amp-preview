#ifndef PREVIEW_COLOUR_HPP
#define PREVIEW_COLOUR_HPP

#include <cstdint>
#include <cstdlib>

namespace infra
{
    enum class PixelFormat
    {
        rgb888,
        rgb565,
        blackandwhite
    };

    enum class Colour : uint32_t
    {
        // Taken from http://www.rapidtables.com/web/color/RGB_Color.htm
        black = 0x00000000,
        white = 0x00ffffff,
        red = 0x00ff0000,
        lime = 0x0000ff00,
        blue = 0x000000ff,
        yellow = 0x00ffff00,
        cyan = 0x0000ffff,
        magenta = 0x00ff00ff,
        silver = 0x00c0c0c0,
        gray = 0x00808080,
        maroon = 0x00800000,
        olive = 0x00808000,
        green = 0x00008000,
        purple = 0x00800080,
        teal = 0x00008080,
        navy = 0x00000080,

        darkRed = 0x008b0000,
        brown = 0x00a52a2a,
        fireBrick = 0x00b22222,
        crimson = 0x00dc143c,
        tomato = 0x00ff6347,
        coral = 0x00ff7f50,
        indianRed = 0x00cd5c5c,
        lightCoral = 0x00f08080,
        darkSalmon = 0x00e9967a,
        salmon = 0x00fa8072,
        lightSalmon = 0x00ffa07a,
        orangeRed = 0x00ff4500,
        darkOrange = 0x00ff8c00,
        orange = 0x00ffa500,
        gold = 0x00ffd700,
        darkGoldenRod = 0x00b8860b,
        goldenRod = 0x00daa520,
        paleGoldenRod = 0x00eee8aa,
        darkKhaki = 0x00bdb76b,
        khaki = 0x00f0e68c,
        yellowGreen = 0x009acd32,
        darkOliveGreen = 0x00556b2f,
        oliveDrab = 0x006b8e23,
        lawnGreen = 0x007cfc00,
        chartreuse = 0x007fff00,
        greenYellow = 0x00adff2f,
        darkGreen = 0x00006400,
        forestGreen = 0x00228b22,
        limeGreen = 0x0032cd32,
        lightGreen = 0x0090ee90,
        paleGreen = 0x0098fb98,
        darkSeaGReen = 0x008fbc8f,
        mediumSpringGreen = 0x0000fa9a,
        springGreen = 0x002e8b57,
        seaGreen = 0x002e8b57,
        mediumAquaMarine = 0x0066cdaa,
        mediumSeaGreen = 0x003cb371,
        lightSeaGreen = 0x0020b2aa,
        darkSlateGray = 0x002f4f4f,
        darkCyan = 0x00008b8b,
        lightCyan = 0x00e0ffff,
        darkTurquoise = 0x0000ced1,
        turquoise = 0x0040e0d0,
        mediumTurquoise = 0x0048d1cc,
        paleTurquoise = 0x00afeeee,
        aquaMarine = 0x007fffd4,
        powderBlue = 0x00b0e0e6,
        cadetBlue = 0x005f9ea0,
        steelBlue = 0x00482b4,
        cornFlowerBlue = 0x006495ed,
        deepSkyBlue = 0x0000bfff,
        dodgerBlue = 0x001e90ff,
        lightBlue = 0x00add8e6,
        skyBlue = 0x0087ceeb,
        lightSkyBlue = 0x0087cefa,
        midnightBlue = 0x00191970,
        darkBlue = 0x0000008b,
        mediumBlue = 0x000000cd,
        royalBlue = 0x004169e1,
        blueViolet = 0x008a2be2,
        indigo = 0x004b0082,
        darkSlateBlue = 0x00483d8b,
        slateBlue = 0x006a5acd,
        mediumSlateBlue = 0xfb68ee,
        mediumPurple = 0x009370db,
        darkMagenta = 0x008b008b,
        darkViolet = 0x009400d3,
        darkOrchid = 0x009932cc,
        mediumOrchid = 0x00ba55d3,
        thistle = 0x00d8bfd8,
        plum = 0x00dda0dd,
        violet = 0x00ee82ee,
        orchid = 0x00da70d6,
        mediumVioletRed = 0x00c71585,
        paleVioletRed = 0x00db7093,
        deepPink = 0x00ff1493,
        hotPink = 0x00ff69b4,
        lightPink = 0x00ffb6c1,
        pink = 0x00ffc0cb,
        antiqueWhite = 0x00faebd7,
        beige = 0x00f5f5dc,
        bisque = 0x00ffe4c4,
        blanchedAlmond = 0x00ffebcd,
        wheat = 0x00f5deb3,
        cornSilk = 0x00fff8dc,
        lemonChiffon = 0x00fffacd,
        lightGoldenRodYellow = 0x00fafad2,
        lightYellow = 0x00ffffe0,
        saddleBrown = 0x008b4513,
        sienna = 0x00a0522d,
        chocolate = 0x00d2691e,
        peru = 0x00cd853f,
        sandyBrown = 0x00f4a460,
        burlyWood = 0x00deb887,
        tan = 0x00d2b48c,
        rosyBrown = 0x00bc8f8f,
        moccasin = 0x00ffe4b5,
        navajoWhite = 0x00ffdead,
        peachPuff = 0x00ffdab9,
        mistyRose = 0x00ffe4e1,
        lavenderBlush = 0x00fff0f5,
        linen = 0x00faf0e6,
        oldLace = 0x00fdf5e6,
        papayaWhip = 0x00ffefd5,
        seaShell = 0x00fff5ee,
        mintCream = 0x00f5fffa,
        slateGray = 0x00708090,
        lightSlateGray = 0x00778899,
        lightSteelBlue = 0x00b0c4de,
        lavender = 0x00e6e6fa,
        floralWhite = 0x00fffaf0,
        aliceBlue = 0x00f0f8ff,
        ghostWhite = 0x00f8f8ff,
        honeyDew = 0x00f0fff0,
        ivory = 0x00fffff0,
        azure = 0x00f0ffff,
        snow = 0x00fffafa,
        dimGray = 0x00696969,
        darkGray = 0x00a9a9a9,
        lightGray = 0x00d3d3d3,
        gainsboro = 0x00dcdcdc,
        whiteSmoke = 0x00f5f5f5
    };

    Colour CreateColour(uint8_t red, uint8_t green, uint8_t blue);
    uint8_t RedFromColour(Colour c);
    uint8_t GreenFromColour(Colour c);
    uint8_t BlueFromColour(Colour c);
    Colour AverageColour(Colour x, Colour y);
    Colour BetweenColour(Colour x, Colour y, uint16_t proportion); // proportion in [0, 256], like a percentage but in a 256 range; 0 = x, 256 = y, 128 is midpoint between x and y

    Colour ConvertRgb565ToRgb888(uint32_t colour);
    uint32_t ConvertRgb888ToRgb565(Colour colour);
    uint32_t ConvertRgb888To(Colour colour, PixelFormat format);
    Colour ConvertToRgb888(uint32_t colour, PixelFormat format);
    bool ConvertRgb565ToBlackAndWhite(uint32_t colour);
    bool ConvertRgb888ToBlackAndWhite(Colour colour);

    constexpr std::size_t PixelSize(PixelFormat pixelFormat)
    {
        return pixelFormat == PixelFormat::rgb888 ? 3u : pixelFormat == PixelFormat::rgb565 ? 2u
                                                                                            : 0u;
    }
}

#endif
