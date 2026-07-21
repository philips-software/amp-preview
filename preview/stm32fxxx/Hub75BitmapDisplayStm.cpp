#include "preview/stm32fxxx/Hub75BitmapDisplayStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "preview/interfaces/BitmapCanvas.hpp"
#include "preview/interfaces/BitmapPainter.hpp"
#include "services/tracer/GlobalTracer.hpp"

namespace hal
{
    Hub75BitmapDisplayStm::Hub75BitmapDisplayStm(hal::DmaStm& dma)
        : dma(dma)
    {
        outputEnable.SetPulse(95, 100);
        timerPwmOutputEnable.StartTimer();
        outputEnable.Start();

        TIM8->DIER |= TIM_DIER_UDE;
        timerDisplay.Start([this]()
            {
                DisplayStep();
            });
    }

    void Hub75BitmapDisplayStm::Paint(services::View& view, infra::Region region, infra::Function<void()> onDone)
    {
        bitmap.Clear();
        hal::BitmapPainterCanonical painter;
        services::BitmapCanvas canvas(bitmap, painter);
        view.Paint(canvas, region);
        painter.WaitUntilDrawingFinished();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void Hub75BitmapDisplayStm::DisplayStep()
    {
        outputEnable.SetDuty(100);

        a.Set((block & 1) != 0);
        b.Set((block & 2) != 0);
        c.Set((block & 4) != 0);
        d.Set((block & 8) != 0);

        auto blockRange = infra::Head(infra::DiscardHead(infra::MakeConst(bitmap.buffer), block * blockSize), blockSize);

        channel.StartTransmit(blockRange);
        timerTransmit.Start();
    }

    void Hub75BitmapDisplayStm::TransferComplete()
    {
        timerTransmit.Stop();

        lat.Set(true);
        lat.Set(false);

        ++block;
        if (block == blockCount)
            block = 0;

        outputEnable.SetDuty(95);
    }
}
