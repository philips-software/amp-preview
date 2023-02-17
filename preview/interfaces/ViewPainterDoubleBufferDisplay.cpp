#include "preview/interfaces/ViewPainterDoubleBufferDisplay.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "preview/interfaces/BitmapCanvas.hpp"

namespace services
{
    ViewPainterDoubleBufferDisplay::ViewPainterDoubleBufferDisplay(hal::DoubleBufferDisplay& display, hal::BitmapPainter& bitmapPainter)
        : display(display)
        , bitmapPainter(bitmapPainter)
    {}

    void ViewPainterDoubleBufferDisplay::Paint(View& view, infra::Region region, infra::Function<void()> onDone)
    {
        this->onDone = onDone;
        this->drawRegion = region;
        services::BitmapCanvas canvas(display.DrawingBitmap(), bitmapPainter);
        view.Paint(canvas, region);
        bitmapPainter.WaitUntilDrawingFinished();
        display.SwapLayers([this]()
            {
            services::BitmapCanvas copyCanvas(display.DrawingBitmap(), bitmapPainter);
            copyCanvas.DrawBitmap(infra::Point(), display.ViewingBitmap(), drawRegion);

            infra::EventDispatcher::Instance().Schedule(this->onDone); });
    }
}
