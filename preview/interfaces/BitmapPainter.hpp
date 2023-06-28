#ifndef PREVIEW_BITMAP_PAINTER_HPP
#define PREVIEW_BITMAP_PAINTER_HPP

#include "infra/util/BoundedString.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/Font.hpp"

namespace hal
{
    class BitmapPainter
    {
    protected:
        BitmapPainter() = default;
        BitmapPainter(const BitmapPainter& other) = delete;
        BitmapPainter& operator=(const BitmapPainter& other) = delete;
        ~BitmapPainter() = default;

    public:
        virtual void DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawLine(infra::Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) = 0;
        virtual void DrawTransparentBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) = 0;
        virtual void DrawIcon(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawString(infra::Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) = 0;
        virtual void WaitUntilDrawingFinished() const = 0;
    };

    class BitmapPainterCanonical
        : public BitmapPainter
    {
    public:
        void DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawLine(infra::Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override;
        void DrawTransparentBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override;
        void DrawIcon(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override;
        void DrawString(infra::Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override;
        void WaitUntilDrawingFinished() const override;

    protected:
        void DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour, infra::Region boundingBox);
        virtual void DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour);
        void DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour, infra::Region boundingBox);
        virtual void DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour);
        infra::Point DrawCharacter(infra::Bitmap& bitmap, infra::Point position, char c, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox);
        void DrawPixel(infra::Bitmap& bitmap, infra::Point position, uint32_t colour);
    };
}

#endif
