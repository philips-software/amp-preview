#ifndef PREVIEW_VIEW_PAINTER_MOCK_HPP
#define PREVIEW_VIEW_PAINTER_MOCK_HPP

#include "preview/interfaces/ViewPainter.hpp"
#include "gmock/gmock.h"

namespace services
{
    class ViewPainterMock
        : public ViewPainter
    {
    public:
        MOCK_METHOD3(Paint, void(View& view, infra::Region region, infra::Function<void()> onDone));
    };
}

#endif
