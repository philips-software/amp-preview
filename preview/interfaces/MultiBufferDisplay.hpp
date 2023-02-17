#ifndef PREVIEW_MULTI_BUFFER_DISPLAY_HPP
#define PREVIEW_MULTI_BUFFER_DISPLAY_HPP

#include "infra/util/Function.hpp"
#include "preview/interfaces/Bitmap.hpp"

namespace hal
{
    class MultiBufferDisplay
    {
    protected:
        MultiBufferDisplay() = default;
        MultiBufferDisplay(const MultiBufferDisplay& other) = delete;
        MultiBufferDisplay& operator=(const MultiBufferDisplay& other) = delete;
        ~MultiBufferDisplay() = default;

    public:
        virtual void SetBitmap(const infra::Bitmap& bitmap, const infra::Function<void()>& onDone) = 0;
    };
}

#endif
