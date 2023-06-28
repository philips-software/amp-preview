#ifndef PREVIEW_VIEW_MOCK_HPP
#define PREVIEW_VIEW_MOCK_HPP

#include "preview/interfaces/View.hpp"
#include "gmock/gmock.h"

namespace services
{
    class ViewMock
        : public View
    {
    public:
        ViewMock(infra::Vector size)
        {
            EXPECT_CALL(*this, ViewRegionChanged());
            SetViewRegion(infra::Region(infra::Point(), size));
        }

        MOCK_CONST_METHOD0(MinimumSize, infra::Vector());
        MOCK_CONST_METHOD0(MaximumSize, infra::Vector());
        MOCK_METHOD2(Paint, void(hal::Canvas& canvas, infra::Region boundingRegion));
        MOCK_METHOD0(ViewRegionChanged, void());
        MOCK_METHOD0(ResetSize, void());
    };

    class TopViewMock
        : public View
    {
    public:
        TopViewMock(View& subView)
            : subView(subView)
        {
            subView.SetParent(*this);
        }

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override
        {
            std::abort();
        }

        MOCK_METHOD1(Dirty, void(infra::Region region));

    private:
        View& subView;
    };
}

#endif
