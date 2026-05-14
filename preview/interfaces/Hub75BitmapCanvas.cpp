#include "preview/interfaces/Hub75BitmapCanvas.hpp"

namespace services
{
    Hub75BitmapCanvas::Hub75BitmapCanvas(infra::Hub75Bitmap& bitmap, hal::Hub75BitmapPainter& bitmapPainter)
        : bitmap(bitmap)
        , bitmapPainter(bitmapPainter)
    {}

    infra::Vector Hub75BitmapCanvas::Size() const
    {
        return bitmap.size;
    }

    void Hub75BitmapCanvas::DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawPixel(bitmap, position, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawLine(bitmap, from, to, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRectangle(bitmap, position, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRectangle(bitmap, position, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRoundedRectangle(bitmap, position, r, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRoundedRectangle(bitmap, position, r, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre, r, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre, rSmall, rLarge, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledCircle(bitmap, centre, r, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        bitmapPainter.DrawBitmap(bitmap, position, sourceBitmap, boundingBox);
    }

    void Hub75BitmapCanvas::DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        bitmapPainter.DrawTransparentBitmap(bitmap, position, sourceBitmap, transparencyColour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawIcon(bitmap, position, sourceBitmap, colour, boundingBox);
    }

    void Hub75BitmapCanvas::DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        bitmapPainter.DrawString(bitmap, position, string, font, colour, direction, boundingBox);
    }
}
