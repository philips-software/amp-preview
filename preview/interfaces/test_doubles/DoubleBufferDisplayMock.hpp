#ifndef PREVIEW_DOUBLE_BUFFER_DISPLAY_MOCK_HPP
#define PREVIEW_DOUBLE_BUFFER_DISPLAY_MOCK_HPP

#include "preview/interfaces/DoubleBufferDisplay.hpp"
#include "gmock/gmock.h"

namespace hal
{
    class DoubleBufferDisplayMock
        : public DoubleBufferDisplay
    {
    public:
        MOCK_METHOD1(SwapLayers, void(const infra::Function<void()>& onDone));
        MOCK_METHOD0(DrawingBitmap, infra::Bitmap&());
        MOCK_CONST_METHOD0(ViewingBitmap, const infra::Bitmap&());
    };
}

#endif
