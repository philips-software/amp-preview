#ifndef PREVIEW_HUB75_BITMAP_PAINTER_HPP
#define PREVIEW_HUB75_BITMAP_PAINTER_HPP

#include "infra/util/BoundedString.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/Hub75Bitmap.hpp"
#include "preview/interfaces/Font.hpp"

namespace hal
{
    class Hub75BitmapPainter
    {
    protected:
        Hub75BitmapPainter() = default;
        Hub75BitmapPainter(const Hub75BitmapPainter& other) = delete;
        Hub75BitmapPainter& operator=(const Hub75BitmapPainter& other) = delete;
        ~Hub75BitmapPainter() = default;

    public:
        virtual void DrawPixel(infra::Hub75Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawLine(infra::Hub75Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRoundedRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRoundedRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawBitmap(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) = 0;
        virtual void DrawTransparentBitmap(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) = 0;
        virtual void DrawIcon(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawString(infra::Hub75Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) = 0;
        virtual void WaitUntilDrawingFinished() const = 0;
    };

    class Hub75BitmapPainterCanonical
        : public Hub75BitmapPainter
    {
    public:
        void DrawPixel(infra::Hub75Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawLine(infra::Hub75Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
        void DrawRoundedRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledRoundedRectangle(infra::Hub75Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override;
        void DrawFilledCircle(infra::Hub75Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
        void DrawBitmap(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override;
        void DrawTransparentBitmap(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override;
        void DrawIcon(infra::Hub75Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override;
        void DrawString(infra::Hub75Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override;
        void WaitUntilDrawingFinished() const override;

    protected:
        void DrawHorizontalLine(infra::Hub75Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour, infra::Region boundingBox);
        virtual void DrawHorizontalLine(infra::Hub75Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour);
        void DrawVerticalLine(infra::Hub75Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour, infra::Region boundingBox);
        virtual void DrawVerticalLine(infra::Hub75Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour);
        infra::Point DrawCharacter(infra::Hub75Bitmap& bitmap, infra::Point position, char c, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox);
        void DrawPixel(infra::Hub75Bitmap& bitmap, infra::Point position, uint32_t colour);
    };
}

#endif
