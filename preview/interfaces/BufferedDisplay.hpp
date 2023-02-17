#ifndef PREVIEW_BUFFERED_DISPLAY_HPP
#define PREVIEW_BUFFERED_DISPLAY_HPP

#include "infra/util/Function.hpp"
#include "preview/interfaces/Bitmap.hpp"

namespace hal
{
    class BufferedDisplay
    {
    protected:
        BufferedDisplay() = default;
        BufferedDisplay(const BufferedDisplay& other) = delete;
        BufferedDisplay& operator=(const BufferedDisplay& other) = delete;
        ~BufferedDisplay() = default;

    public:
        virtual infra::Region DisplayRegion() const = 0;
        virtual infra::PixelFormat PixelFormat() const = 0;
        virtual void DrawBitmap(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone) = 0;
    };
}

#endif
