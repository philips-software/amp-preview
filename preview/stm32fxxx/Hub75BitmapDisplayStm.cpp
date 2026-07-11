#include "preview/stm32fxxx/Hub75BitmapDisplayStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "preview/interfaces/BitmapCanvas.hpp"
#include "preview/interfaces/BitmapPainter.hpp"

namespace hal
{
    Hub75BitmapDisplayStm::Hub75BitmapDisplayStm()
    {}

    Hub75BitmapDisplayStm::~Hub75BitmapDisplayStm()
    {}

    void Hub75BitmapDisplayStm::Paint(services::View& view, infra::Region region, infra::Function<void()> onDone)
    {
        bitmap.Clear();
        hal::BitmapPainterCanonical painter;
        services::BitmapCanvas canvas(bitmap, painter);
        view.Paint(canvas, region);
        painter.WaitUntilDrawingFinished();

        //infra::EventDispatcher::Instance().Schedule(onDone);
    }
}
