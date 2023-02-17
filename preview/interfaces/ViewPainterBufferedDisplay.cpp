#include "preview/interfaces/ViewPainterBufferedDisplay.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "preview/interfaces/WindowBitmapCanvas.hpp"

namespace services
{
    ViewPainterBufferedDisplay::ViewPainterBufferedDisplay(infra::ByteRange buffer, hal::BufferedDisplay& display, hal::BitmapPainter& bitmapPainter)
        : display(display)
        , buffer(buffer)
        , bitmapPainter(bitmapPainter)
    {}

    void ViewPainterBufferedDisplay::Paint(View& view, infra::Region region, infra::Function<void()> onDone)
    {
        region = infra::Intersection(region, display.DisplayRegion());

        if (!region.Empty())
        {
            infra::ByteRange bitmapBuffer(infra::Head(buffer, infra::Bitmap::BufferSize(region.Size(), display.PixelFormat())));
            infra::Bitmap bitmap(bitmapBuffer, region.Size(), display.PixelFormat());
            services::WindowBitmapCanvas canvas(view.ViewRegion().Size(), bitmap, infra::Point() - region.TopLeft(), bitmapPainter);

            view.Paint(canvas, region);
            bitmapPainter.WaitUntilDrawingFinished();
            display.DrawBitmap(bitmap, region.TopLeft(), onDone);
        }
        else
            infra::EventDispatcher::Instance().Schedule(onDone);
    }
}
