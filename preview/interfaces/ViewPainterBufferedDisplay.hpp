#ifndef PREVIEW_VIEW_PAINTER_BUFFERED_DISPLAY_HPP
#define PREVIEW_VIEW_PAINTER_BUFFERED_DISPLAY_HPP

#include "infra/util/WithStorage.hpp"
#include "preview/interfaces/BitmapPainter.hpp"
#include "preview/interfaces/BufferedDisplay.hpp"
#include "preview/interfaces/ViewPainter.hpp"

namespace services
{
    class ViewPainterBufferedDisplay
        : public services::ViewPainter
    {
    public:
        template<std::size_t X, std::size_t Y, infra::PixelFormat Format>
        using WithBuffer = infra::WithStorage<ViewPainterBufferedDisplay, std::array<uint8_t, Format == infra::PixelFormat::blackandwhite ? (X* Y + 7) / 8 : X * Y * infra::PixelSize(Format)>>;

        ViewPainterBufferedDisplay(infra::ByteRange buffer, hal::BufferedDisplay& display, hal::BitmapPainter& bitmapPainter);

        virtual void Paint(View& view, infra::Region region, infra::Function<void()> onDone) override;

    private:
        hal::BufferedDisplay& display;
        infra::ByteRange buffer;
        hal::BitmapPainter& bitmapPainter;
    };
}

#endif
