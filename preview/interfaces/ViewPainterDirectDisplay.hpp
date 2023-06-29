#ifndef PREVIEW_VIEW_PAINTER_DIRECT_DISPLAY_HPP
#define PREVIEW_VIEW_PAINTER_DIRECT_DISPLAY_HPP

#include "preview/interfaces/DirectDisplay.hpp"
#include "preview/interfaces/ViewPainter.hpp"

namespace services
{
    class ViewPainterDirectDisplay
        : public services::ViewPainter
    {
    public:
        explicit ViewPainterDirectDisplay(hal::DirectDisplay& canvas);

        void Paint(View& view, infra::Region region, infra::Function<void()> onDone) override;

    private:
        hal::DirectDisplay& display;
    };
}

#endif
