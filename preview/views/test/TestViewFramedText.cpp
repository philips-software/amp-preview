#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/ViewFramedText.hpp"
#include "gmock/gmock.h"

class ViewFramedTextTest
    : public testing::Test
{
public:
    ViewFramedTextTest()
        : view(services::FramedTextAttributes{ infra::Colour::blue, infra::Colour::red }, "Test")
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 31))));
        view.Resize(infra::Vector(80, 30));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewFramedText view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewFramedTextTest, Paint_paints_view)
{
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 28))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFramedTextTest, SetFrameColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 30))));
    view.SetFrameColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 28))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFramedTextTest, SetBackgroundColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 30))));
    view.SetBackgroundColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 28))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}
