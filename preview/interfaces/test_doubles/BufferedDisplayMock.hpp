#ifndef PREVIEW_BUFFERED_DISPLAY_MOCK_HPP
#define PREVIEW_BUFFERED_DISPLAY_MOCK_HPP

#include "preview/interfaces/BufferedDisplay.hpp"
#include "gmock/gmock.h"

namespace hal
{
    class BufferedDisplayMock
        : public BufferedDisplay
    {
    public:
        MOCK_CONST_METHOD0(DisplayRegion, infra::Region());
        MOCK_CONST_METHOD0(PixelFormat, infra::PixelFormat());
        MOCK_METHOD3(DrawBitmap, void(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone));
    };
}

#endif
