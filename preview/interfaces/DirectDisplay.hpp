#ifndef PREVIEW_DIRECT_DISPLAY_HPP
#define PREVIEW_DIRECT_DISPLAY_HPP

#include "preview/interfaces/Canvas.hpp"

namespace hal
{
    class DirectDisplay
        : public hal::Canvas
    {
    protected:
        DirectDisplay() = default;
        DirectDisplay(const DirectDisplay& other) = delete;
        DirectDisplay& operator=(const DirectDisplay& other) = delete;
        ~DirectDisplay() = default;

    public:
        virtual void PaintingComplete() = 0;
    };
}

#endif
