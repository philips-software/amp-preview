#include "gmock/gmock.h"
#include "preview/views/ViewFrame.hpp"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

class ViewFrameTest
    : public testing::Test
{
public:
    ViewFrameTest()
        : view(services::FrameAttributes{ infra::Colour::blue, infra::Colour::red })
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 30))));
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(1, 1), infra::Vector(78, 28))));
        view.Resize(infra::Vector(80, 30));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewFrame view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewFrameTest, Paint_paints_view_and_subview)
{
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 1), infra::Vector(78, 28)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFrameTest, SetFrameColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty(view.ViewRegion()));
    view.SetFrameColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 1), infra::Vector(78, 28)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFrameTest, SetBackgroundColour_changes_frame_colour)
{
    EXPECT_CALL(topView, Dirty((view.ViewRegion() >> infra::Vector(1, 1)) - infra::Vector(2, 2)));
    view.SetBackgroundColour(infra::Colour::lightBlue);
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 1), infra::Vector(78, 28)), infra::Colour::lightBlue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFrameTest, Move_moves_view)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(81, 32))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(1, 1), infra::Vector(79, 30))));
    view.Move(infra::Point(1, 2));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(2, 3), infra::Vector(78, 28)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(1, 2), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFrameTest, ViewRegion)
{
    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(80, 30)), view.ViewRegion());
}

TEST_F(ViewFrameTest, MinimumSize)
{
    EXPECT_EQ(infra::Vector(2, 2), view.MinimumSize());
}

TEST_F(ViewFrameTest, MaximumSize)
{
    EXPECT_EQ(infra::Vector::Maximum(), view.MaximumSize());
}

class ViewFrameWithSubViewTest
    : public testing::Test
{
public:
    ViewFrameWithSubViewTest()
        : view(services::FrameAttributes{ infra::Colour::blue, infra::Colour::red }, infra::Vector())
    {
        EXPECT_CALL(view.SubView(), ViewRegionChanged());
        view.Resize(infra::Vector(80, 30));
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewFrame::WithView<testing::StrictMock<services::ViewMock>> view;
};

TEST_F(ViewFrameWithSubViewTest, Paint_paints_view_and_subview)
{
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(1, 1), infra::Vector(78, 28)), infra::Colour::red, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    EXPECT_CALL(canvas, DrawRectangle(infra::Region(infra::Point(0, 0), infra::Vector(80, 30)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(80, 30))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(80, 30)));
}

TEST_F(ViewFrameWithSubViewTest, ResetSize_resets_subview_size)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    view.SubView().Resize(infra::Vector(150, 200));

    EXPECT_CALL(view.SubView(), ResetSize());
    view.ResetSize();

    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(152, 202)), view.ViewRegion());
}
