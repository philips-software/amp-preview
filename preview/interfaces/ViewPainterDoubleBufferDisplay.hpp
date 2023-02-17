#ifndef PREVIEW_VIEW_PAINTER_DOUBLE_BUFFER_HPP
#define PREVIEW_VIEW_PAINTER_DOUBLE_BUFFER_HPP

#include "preview/interfaces/BitmapPainter.hpp"
#include "preview/interfaces/DoubleBufferDisplay.hpp"
#include "preview/interfaces/ViewPainter.hpp"

namespace services
{
    class ViewPainterDoubleBufferDisplay
        : public services::ViewPainter
    {
    public:
        ViewPainterDoubleBufferDisplay(hal::DoubleBufferDisplay& display, hal::BitmapPainter& bitmapPainter);

        virtual void Paint(View& view, infra::Region region, infra::Function<void()> onDone) override;

    private:
        hal::DoubleBufferDisplay& display;
        hal::BitmapPainter& bitmapPainter;
        infra::Function<void()> onDone;
        infra::Region drawRegion;
    };
}

#endif
