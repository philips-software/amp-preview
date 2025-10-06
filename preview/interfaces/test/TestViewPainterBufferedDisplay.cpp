#include "infra/event/test_helper/EventDispatcherFixture.hpp"
#include "preview/interfaces/ViewPainterBufferedDisplay.hpp"
#include "preview/interfaces/test_doubles/BitmapPainterMock.hpp"
#include "preview/interfaces/test_doubles/BufferedDisplayMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "gmock/gmock.h"

class ViewPainterBufferedDisplayTest
    : public testing::Test
    , public infra::EventDispatcherFixture
{
public:
    ViewPainterBufferedDisplayTest()
        : view(infra::Vector(4, 8))
    {
        EXPECT_CALL(display, PixelFormat()).WillRepeatedly(testing::Return(infra::PixelFormat::rgb565));

        viewPainter.Emplace(buffer, display, bitmapPainter);
    }

    std::array<uint8_t, 8 * 16 * 2> buffer;
    testing::StrictMock<hal::BufferedDisplayMock> display;
    testing::StrictMock<hal::BitmapPainterMock> bitmapPainter;
    testing::StrictMock<services::ViewMock> view;
    std::optional<services::ViewPainterBufferedDisplay> viewPainter;
};

TEST_F(ViewPainterBufferedDisplayTest, Paint)
{
    testing::InSequence s;

    infra::Region dirtyRegion(infra::Point(), infra::Vector(2, 3));

    EXPECT_CALL(display, DisplayRegion()).WillOnce(testing::Return(infra::Region(infra::Point(), infra::Vector(10, 10))));
    EXPECT_CALL(view, Paint(testing::_, dirtyRegion));
    EXPECT_CALL(bitmapPainter, WaitUntilDrawingFinished());
    EXPECT_CALL(display, DrawBitmap(testing::_, dirtyRegion.TopLeft(), testing::_));
    viewPainter->Paint(view, dirtyRegion, infra::emptyFunction);
}

TEST_F(ViewPainterBufferedDisplayTest, when_nothing_is_dirty_no_painting_is_done)
{
    testing::InSequence s;

    EXPECT_CALL(display, DisplayRegion()).WillOnce(testing::Return(infra::Region(infra::Point(), infra::Vector(10, 10))));
    viewPainter->Paint(view, infra::Region(), infra::emptyFunction);
}
