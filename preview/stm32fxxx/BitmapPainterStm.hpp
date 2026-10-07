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

        void DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::SimpleBitmap& sourceBitmap, infra::Region boundingBox) override final;
        void WaitUntilDrawingFinished() const override final;

    private:
        void DrawHorizontalLine(infra::SimpleBitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour) override;
        void DrawVerticalLine(infra::SimpleBitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour) override;
    };
}

#endif
