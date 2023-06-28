#include "infra/event/test_helper/EventDispatcherFixture.hpp"
#include "preview/interfaces/ViewPainterDirectDisplay.hpp"
#include "preview/interfaces/test_doubles/DirectDisplayMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "gmock/gmock.h"

class ViewPainterDirectDisplay
    : public testing::Test
    , public infra::EventDispatcherFixture
{
public:
    ViewPainterDirectDisplay()
        : viewPainter(display)
        , view(infra::Vector(4, 8))
    {}

    testing::StrictMock<hal::DirectDisplayMock> display;
    testing::StrictMock<services::ViewMock> view;
    services::ViewPainterDirectDisplay viewPainter;
};

TEST_F(ViewPainterDirectDisplay, Paint)
{
    testing::InSequence s;

    infra::Region dirtyRegion(infra::Point(), infra::Vector(2, 3));

    EXPECT_CALL(view, Paint(testing::Ref(display), dirtyRegion));
    EXPECT_CALL(display, PaintingComplete());
    viewPainter.Paint(view, dirtyRegion, infra::emptyFunction);
}
