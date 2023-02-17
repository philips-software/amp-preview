#ifndef PREVIEW_BITMAP_PAINTER_STM_HPP
#define PREVIEW_BITMAP_PAINTER_STM_HPP

#include "preview/interfaces/BitmapPainter.hpp"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

namespace hal
{
    class BitmapPainterStm
        : public BitmapPainterCanonical
    {
    public:
        BitmapPainterStm();
        ~BitmapPainterStm();

        virtual void DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) override final;
        virtual void DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override final;
        virtual void DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override final;
        virtual void WaitUntilDrawingFinished() const override;

    private:
        virtual void DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour) override;
        virtual void DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour) override;
    };
}

#endif
