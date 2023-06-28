#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/ViewPanel.hpp"
#include "gmock/gmock.h"

class ViewPanelWithoutSubViewTest
    : public testing::Test
{
public:
    ViewPanelWithoutSubViewTest()
        : view(infra::Colour::blue)
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(10, 20))));
        view.Resize(infra::Vector(10, 20));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewPanel view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewPanelWithoutSubViewTest, Paint_paints_view)
{
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(100, 100)));
}

TEST_F(ViewPanelWithoutSubViewTest, SetColour_makes_bitmap_dirty)
{
    // build
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(0, 0, 100, 100));

    // operate
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(10, 20))));
    view.SetColour(infra::Colour::green);

    // check
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::green, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(0, 0, 100, 100));
}

TEST_F(ViewPanelWithoutSubViewTest, SetViewRegion_is_not_forwarded)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(10, 20))));
    view.SetViewRegion(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)));
}

TEST_F(ViewPanelWithoutSubViewTest, ViewRegion_is_not_forwarded)
{
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), view.ViewRegion());
}

TEST_F(ViewPanelWithoutSubViewTest, MinimumSize_is_not_forwarded)
{
    EXPECT_EQ(infra::Vector(), view.MinimumSize());
}

TEST_F(ViewPanelWithoutSubViewTest, MaximumSize_is_not_forwarded)
{
    EXPECT_EQ(infra::Vector::Maximum(), view.MaximumSize());
}

class ViewPanelTest
    : public testing::Test
{
public:
    ViewPanelTest()
        : view(infra::Colour::blue, infra::Vector())
    {
        EXPECT_CALL(view.SubView(), ViewRegionChanged());
        view.Resize(infra::Vector(10, 20));
    }

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewPanel::WithView<testing::StrictMock<services::ViewMock>> view;
};

TEST_F(ViewPanelTest, Paint_paints_view_and_subview)
{
    testing::InSequence s;

    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(infra::Point(0, 0), infra::Vector(100, 100)));
}

TEST_F(ViewPanelTest, SetColour_makes_bitmap_dirty)
{
    // build
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::blue, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(0, 0, 100, 100));

    // operate
    view.SetColour(infra::Colour::green);

    // check
    EXPECT_CALL(canvas, DrawFilledRectangle(infra::Region(infra::Point(0, 0), infra::Vector(10, 20)), infra::Colour::green, infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(infra::Point(0, 0), infra::Vector(100, 100))));
    view.Paint(canvas, infra::Region(0, 0, 100, 100));
}

TEST_F(ViewPanelTest, SetViewRegion_is_forwarded)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    view.SetViewRegion(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)), view.SubView().ViewRegion());
}

TEST_F(ViewPanelTest, MinimumSize_is_forwarded)
{
    EXPECT_CALL(view.SubView(), MinimumSize()).WillOnce(testing::Return(infra::Vector(1, 2)));
    EXPECT_EQ(infra::Vector(1, 2), view.MinimumSize());
}

TEST_F(ViewPanelTest, MaximumSize_is_forwarded)
{
    EXPECT_CALL(view.SubView(), MaximumSize()).WillOnce(testing::Return(infra::Vector(1, 2)));
    EXPECT_EQ(infra::Vector(1, 2), view.MaximumSize());
}

TEST_F(ViewPanelTest, ResetSize_resets_subview_size)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    view.SubView().Resize(infra::Vector(150, 200));

    EXPECT_CALL(view.SubView(), ResetSize());
    view.ResetSize();

    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(150, 200)), view.ViewRegion());
}
