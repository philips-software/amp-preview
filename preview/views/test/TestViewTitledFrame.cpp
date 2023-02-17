#include "gmock/gmock.h"
#include "preview/views/ViewTitledFrame.hpp"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

class ViewTitledFrameTest
    : public testing::Test
{
public:
    ViewTitledFrameTest()
        : view(services::TitledFrameAttributes{ infra::Colour::blue, infra::Colour::red }, "Test")
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 40))));
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 31))));
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(0, 28), infra::Vector(80, 12))));
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(1, 29), infra::Vector(78, 10))));
        view.Resize(infra::Vector(80, 40));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewTitledFrame view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewTitledFrameTest, Paint_paints_view)
{
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 27))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 29), infra::Vector(78, 10)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 28), infra::Vector(80, 12)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 40)));
}

TEST_F(ViewTitledFrameTest, SetFrameColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 29))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(0, 28), infra::Vector(80, 12))));
    view.SetFrameColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 27))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 29), infra::Vector(78, 10)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 28), infra::Vector(80, 12)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 40)));
}

TEST_F(ViewTitledFrameTest, SetBackgroundColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 29))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(1, 29), infra::Vector(78, 10))));
    view.SetBackgroundColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 27))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 29), infra::Vector(78, 10)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 28), infra::Vector(80, 12)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 40)));
}

TEST_F(ViewTitledFrameTest, Move_moves_view)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(81, 42))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(81, 31))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(0, 28), infra::Vector(81, 14))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(1, 29), infra::Vector(79, 12))));
    view.Move(infra::Point(1, 2));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 2), infra::Vector(80, 29)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(1, 2), infra::Vector(80, 29)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawString(infra::Point(17, 25), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(2, 3), infra::Vector(78, 27))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(2, 31), infra::Vector(78, 10)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(1, 30), infra::Vector(80, 12)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 40)));
}

TEST_F(ViewTitledFrameTest, ViewRegion)
{
    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(80, 40)), view.ViewRegion());
}

TEST_F(ViewTitledFrameTest, MinimumSize)
{
    EXPECT_EQ(infra::Vector(2, 3), view.MinimumSize());
}

TEST_F(ViewTitledFrameTest, MaximumSize)
{
    EXPECT_EQ(infra::Vector::Maximum(), view.MaximumSize());
}

class ViewTitledFrameWithSubViewTest
    : public testing::Test
{
public:
    ViewTitledFrameWithSubViewTest()
        : view(services::TitledFrameAttributes{ infra::Colour::blue, infra::Colour::red }, "Test", infra::Vector())
    {
        EXPECT_CALL(view.SubView(), ViewRegionChanged());
        view.Resize(infra::Vector(80, 40));
        EXPECT_EQ(infra::Region(infra::Point(1, 29), infra::Vector(78, 10)), view.SubView().ViewRegion());
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewTitledFrame::WithView<testing::StrictMock<services::ViewMock>> view;
};

TEST_F(ViewTitledFrameWithSubViewTest, SetSubView_sets_view)
{
    testing::InSequence s;

    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 29)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawString(infra::Point(16, 23), infra::BoundedConstString("Test"), testing::_, infra::Colour::white, infra::RightAngle::angle_0, infra::Region(infra::Point(1, 1), infra::Vector(78, 27))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 29), infra::Vector(78, 10)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 28), infra::Vector(80, 12)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 40))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 40)));
}

TEST_F(ViewTitledFrameWithSubViewTest, ResetSize_resets_subview_size)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    view.SubView().Resize(infra::Vector(150, 200));

    EXPECT_CALL(view.SubView(), ResetSize());
    view.ResetSize();

    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(152, 230)), view.ViewRegion());
}
