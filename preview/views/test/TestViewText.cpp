#include "gmock/gmock.h"
#include "preview/views/ViewText.hpp"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

class ViewTextTest
    : public testing::Test
{
public:
    ViewTextTest()
        : view(services::TextAttributes(infra::Colour::white), "Test")
    {
        EXPECT_EQ(infra::Vector(47, 29), view.ViewRegion().Size());
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 30))));
        view.Resize(infra::Vector(80, 30));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewText view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewTextTest, Paint_paints_view)
{
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, SetText_changes_text)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetText("New");
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("New"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, SetTextColour_changes_colour)
{
    EXPECT_CALL(topView, Dirty(view.TextRegion()));
    view.SetTextColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::lightBlue, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, alignment_left)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetHorizontalAlignment(services::HorizontalAlignment::left);
    EXPECT_CALL(canvas, DrawString(infra::Point(0, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, alignment_right)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetHorizontalAlignment(services::HorizontalAlignment::right);
    EXPECT_CALL(canvas, DrawString(infra::Point(33, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, alignment_top)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetVerticalAlignment(services::VerticalAlignment::top);
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 18), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, alignment_bottom)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetVerticalAlignment(services::VerticalAlignment::bottom);
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 29), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewTextTest, change_view_region_changes_text_position)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(100, 40))));
    view.SetViewRegion(infra::Region(infra::Point(), infra::Vector(100, 40)));
    EXPECT_CALL(canvas, DrawString(infra::Point(26, 28), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(0, 0), infra::Vector(100, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(100, 40)));
}
