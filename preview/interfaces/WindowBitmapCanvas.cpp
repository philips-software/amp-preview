#include "preview/interfaces/WindowBitmapCanvas.hpp"

namespace services
{
    WindowBitmapCanvas::WindowBitmapCanvas(infra::Vector size, infra::Bitmap& bitmap, infra::Vector shift, hal::BitmapPainter& bitmapPainter)
        : size(size)
        , bitmap(bitmap)
        , shift(shift)
        , bitmapPainter(bitmapPainter)
    {}

    infra::Vector WindowBitmapCanvas::Size() const
    {
        return size;
    }

    void WindowBitmapCanvas::DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawPixel(bitmap, position + shift, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawLine(bitmap, from + shift, to + shift, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRectangle(bitmap, position >> shift, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRectangle(bitmap, position >> shift, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawRoundedRectangle(bitmap, position >> shift, r, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledRoundedRectangle(bitmap, position >> shift, r, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre + shift, r, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawCircle(bitmap, centre + shift, rSmall, rLarge, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawFilledCircle(bitmap, centre + shift, r, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        bitmapPainter.DrawBitmap(bitmap, position + shift, sourceBitmap, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        bitmapPainter.DrawTransparentBitmap(bitmap, position + shift, sourceBitmap, transparencyColour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        bitmapPainter.DrawIcon(bitmap, position + shift, sourceBitmap, colour, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }

    void WindowBitmapCanvas::DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        bitmapPainter.DrawString(bitmap, position + shift, string, font, colour, direction, (boundingBox >> shift) & infra::Region(infra::Point(), bitmap.size));
    }
}
