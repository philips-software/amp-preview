#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/VerticalLayout.hpp"
#include "gmock/gmock.h"

class VerticalLayoutTest
    : public testing::Test
{
public:
    VerticalLayoutTest()
        : layout(5, 4, 3, 2, 1)
        , view1(infra::Vector(10, 20))
        , view2(infra::Vector(30, 40))
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 100))));
        layout.Resize(infra::Vector(80, 100));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<services::ViewMock> view1;
    testing::StrictMock<services::ViewMock> view2;
    services::VerticalLayout::WithMaxViews<5> layout;
    testing::StrictMock<hal::CanvasMock> canvas;
    testing::StrictMock<services::TopViewMock> topView{ layout };
};

TEST_F(VerticalLayoutTest, Paint_empty_layout)
{
    EXPECT_EQ(infra::Vector(4, 6), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(32767, 32767), layout.MaximumSize());
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}

TEST_F(VerticalLayoutTest, Paint_layout_with_one_view)
{
    layout.Add(view1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 24))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(10, 20)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(14, 26), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(32767, 26), layout.MaximumSize());

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}

TEST_F(VerticalLayoutTest, add_centre_aligned_view)
{
    layout.Add(view1, 0, services::HorizontalAlignment::centre);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(44, 24))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(34, 4), infra::Vector(10, 20)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(14, 26), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(32767, 26), layout.MaximumSize());
}

TEST_F(VerticalLayoutTest, add_fill_aligned_view)
{
    layout.AddFill(view1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(10, 5)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(77, 24))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(76, 20)), view1.ViewRegion());

    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(10, 5)));
    EXPECT_EQ(infra::Vector(14, 26), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(1004, 26), layout.MaximumSize());
}

TEST_F(VerticalLayoutTest, add_two_proportional_views)
{
    layout.Add(view1, 1);
    layout.Add(view2, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 48))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(31, 98))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(10, 44)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(1, 53), infra::Vector(30, 45)), view2.ViewRegion());

    EXPECT_EQ(infra::Vector(34, 11), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(32767, 2011), layout.MaximumSize());
}

TEST_F(VerticalLayoutTest, add_two_proportional_views_with_minimum_size_restriction)
{
    layout.Add(view1, 1);
    layout.Add(view2, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 50)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 43))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(31, 98))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(10, 39)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(1, 48), infra::Vector(30, 50)), view2.ViewRegion());
}

TEST_F(VerticalLayoutTest, add_proportional_view_with_dummy)
{
    layout.Add(view1, 1);
    layout.AddDummy(1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 48))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(10, 44)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(14, 11), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(32767, 1011), layout.MaximumSize());
}

TEST_F(VerticalLayoutTest, bring_view_to_front)
{
    layout.Add(view1);
    layout.Add(view2);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 24))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(31, 69))));
    layout.ResetLayout();

    testing::InSequence s;

    layout.BringToFront(view2);
    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    EXPECT_CALL(view2, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(100, 80)));
}

TEST_F(VerticalLayoutTest, MinimumSize_is_respected)
{
    layout.Add(view1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(100, 100)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));

    EXPECT_EQ(infra::Vector(104, 106), layout.MinimumSize());

    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(101, 104))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(100, 100)), view1.ViewRegion());
}

TEST_F(VerticalLayoutTest, MaximumSize_is_respected)
{
    layout.AddFill(view1, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(10, 10)));

    EXPECT_EQ(infra::Vector(14, 16), layout.MaximumSize());

    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(11, 20))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(1, 4), infra::Vector(10, 10)), view1.ViewRegion());
}
