#ifndef PREVIEW_DOUBLE_BUFFER_DISPLAY_HPP
#define PREVIEW_DOUBLE_BUFFER_DISPLAY_HPP

#include "infra/util/Function.hpp"
#include "preview/interfaces/Bitmap.hpp"

namespace hal
{
    class DoubleBufferDisplay
    {
    protected:
        DoubleBufferDisplay() = default;
        DoubleBufferDisplay(const DoubleBufferDisplay& other) = delete;
        DoubleBufferDisplay& operator=(const DoubleBufferDisplay& other) = delete;
        ~DoubleBufferDisplay() = default;

    public:
        virtual void SwapLayers(const infra::Function<void()>& onDone) = 0;
        virtual infra::Bitmap& DrawingBitmap() = 0;
        virtual const infra::Bitmap& ViewingBitmap() const = 0;
    };
}

#endif
