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

        void DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox) final;
        void DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox) final;
        void DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) final;
        void WaitUntilDrawingFinished() const override;

    private:
        void DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour) override;
        void DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour) override;
    };
}

#endif
