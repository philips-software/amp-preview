#ifndef PREVIEW_BITMAP_PAINTER_MOCK_HPP
#define PREVIEW_BITMAP_PAINTER_MOCK_HPP

#include "infra/util/test_helper/BoundedStringMatcher.hpp"
#include "preview/interfaces/BitmapPainter.hpp"

namespace hal
{
    class BitmapPainterMock
        : public BitmapPainter
    {
    public:
        MOCK_METHOD4(DrawPixel, void(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawLine, void(infra::Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawRectangle, void(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawFilledRectangle, void(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawRoundedRectangle, void(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawFilledRoundedRectangle, void(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawCircle, void(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD6(DrawCircle, void(infra::Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD5(DrawFilledCircle, void(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD4(DrawBitmap, void(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox));
        MOCK_METHOD5(DrawTransparentBitmap, void(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox));
        MOCK_METHOD5(DrawIcon, void(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox));
        MOCK_METHOD7(DrawString, void(infra::Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox));
        MOCK_CONST_METHOD0(WaitUntilDrawingFinished, void());
    };
}

#endif
