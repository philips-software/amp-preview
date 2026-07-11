#ifndef PREVIEW_HUB75_BITMAP_DISPLAY_STM_HPP
#define PREVIEW_HUB75_BITMAP_DISPLAY_STM_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "preview/interfaces/ViewPainter.hpp"
#include "preview/interfaces/Hub75Bitmap.hpp"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

namespace hal
{
    class Hub75BitmapDisplayStm
        : public services::ViewPainter
    {
    public:
        Hub75BitmapDisplayStm();
        ~Hub75BitmapDisplayStm();

        virtual void Paint(services::View& view, infra::Region region, infra::Function<void()> onDone) override;

    private:
        infra::Hub75Bitmap::WithStorage<64, 32> bitmap;
    };
}

#endif
