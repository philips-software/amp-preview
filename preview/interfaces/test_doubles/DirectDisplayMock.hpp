#ifndef PREVIEW_DIRECT_DISPLAY_MOCK_HPP
#define PREVIEW_DIRECT_DISPLAY_MOCK_HPP

#include "preview/interfaces/DirectDisplay.hpp"

namespace hal
{
    class DirectDisplayMock
        : public hal::DirectDisplay
    {
    public:
        MOCK_CONST_METHOD0(Size, infra::Vector());
        MOCK_METHOD3(DrawPixel, void(infra::Point position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawLine, void(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD3(DrawRectangle, void(infra::Region position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD3(DrawFilledRectangle, void(infra::Region position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawRoundedRectangle, void(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawFilledRoundedRectangle, void(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawCircle, void(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawCircle, void(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawFilledCircle, void(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD3(DrawBitmap, void(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox));
        MOCK_METHOD4(DrawTransparentBitmap, void(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox));
        MOCK_METHOD4(DrawIcon, void(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD6(DrawString, void(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox));
        MOCK_METHOD0(PaintingComplete, void());
    };
}

#endif
