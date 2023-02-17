#include "preview/interfaces/ViewPainterDirectDisplay.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace services
{
    ViewPainterDirectDisplay::ViewPainterDirectDisplay(hal::DirectDisplay& display)
        : display(display)
    {}

    void ViewPainterDirectDisplay::Paint(View& view, infra::Region region, infra::Function<void()> onDone)
    {
        view.Paint(display, region);
        display.PaintingComplete();
        infra::EventDispatcher::Instance().Schedule(onDone);
    }
}
