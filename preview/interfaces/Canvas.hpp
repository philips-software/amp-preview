#ifndef PREVIEW_CANVAS_HPP
#define PREVIEW_CANVAS_HPP

#include "infra/util/BoundedString.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/Font.hpp"

namespace hal
{
    class Canvas
    {
    protected:
        Canvas() = default;
        Canvas(const Canvas& other) = delete;
        Canvas& operator=(const Canvas& other) = delete;
        ~Canvas() = default;

    public:
        virtual infra::Vector Size() const = 0;

        virtual void DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) = 0;
        virtual void DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) = 0;
        virtual void DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) = 0;
        virtual void DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) = 0;
    };
}

#endif
