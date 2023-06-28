#ifndef PREVIEW_BITMAP_CANVAS_HPP
#define PREVIEW_BITMAP_CANVAS_HPP

#include "preview/interfaces/BitmapPainter.hpp"
#include "preview/interfaces/Canvas.hpp"

namespace services
{
    class BitmapCanvas
        : public hal::Canvas
    {
    public:
        BitmapCanvas(infra::Bitmap& bitmap, hal::BitmapPainter& bitmapPainter);

    public:
        infra::Vector Size() const override;
        void DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override;
        void DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override;
        void DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override;
        void DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override;

    private:
        infra::Bitmap& bitmap;
        hal::BitmapPainter& bitmapPainter;
    };
}

#endif
