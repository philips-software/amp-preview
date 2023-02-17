#include "preview/interfaces/BitmapCanvas.hpp"

namespace services
{
    BitmapCanvas::BitmapCanvas(infra::Bitmap& bitmap, hal::BitmapPainter& bitmapPainter)
        : bitmap(bitmap)
        , bitmapPainter(bitmapPainter)
    {}

    infra::Vector BitmapCanvas::Size() const
    {
        return bitmap.size;
    }

    void BitmapCanvas::DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawPixel(bitmap, position, colour, boundingBox);
    }

    void BitmapCanvas::DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawLine(bitmap, from, to, colour, boundingBox);
    }

    void BitmapCanvas::DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRectangle(bitmap, position, colour, boundingBox);
    }

    void BitmapCanvas::DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRectangle(bitmap, position, colour, boundingBox);
    }

    void BitmapCanvas::DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRoundedRectangle(bitmap, position, r, colour, boundingBox);
    }

    void BitmapCanvas::DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRoundedRectangle(bitmap, position, r, colour, boundingBox);
    }

    void BitmapCanvas::DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre, r, colour, boundingBox);
    }

    void BitmapCanvas::DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre, rSmall, rLarge, colour, boundingBox);
    }

    void BitmapCanvas::DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledCircle(bitmap, centre, r, colour, boundingBox);
    }

    void BitmapCanvas::DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        bitmapPainter.DrawBitmap(bitmap, position, sourceBitmap, boundingBox);
    }

    void BitmapCanvas::DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        bitmapPainter.DrawTransparentBitmap(bitmap, position, sourceBitmap, transparencyColour, boundingBox);
    }

    void BitmapCanvas::DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawIcon(bitmap, position, sourceBitmap, colour, boundingBox);
    }

    void BitmapCanvas::DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        bitmapPainter.DrawString(bitmap, position, string, font, colour, direction, boundingBox);
    }
}
