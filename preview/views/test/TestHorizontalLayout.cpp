#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/HorizontalLayout.hpp"
#include "gmock/gmock.h"

class HorizontalLayoutTest
    : public testing::Test
{
public:
    HorizontalLayoutTest()
        : layout(5, 1, 2, 3, 4)
        , view1(infra::Vector(20, 10))
        , view2(infra::Vector(40, 30))
    {
        EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(100, 80))));
        layout.Resize(infra::Vector(100, 80));
        testing::Mock::VerifyAndClearExpectations(&topView);
    }

    testing::StrictMock<services::ViewMock> view1;
    testing::StrictMock<services::ViewMock> view2;
    services::HorizontalLayout::WithMaxViews<5> layout;
    testing::StrictMock<hal::CanvasMock> canvas;
    testing::StrictMock<services::TopViewMock> topView{ layout };
};

TEST_F(HorizontalLayoutTest, Paint_empty_layout)
{
    EXPECT_EQ(infra::Vector(6, 4), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(32767, 32767), layout.MaximumSize());
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(100, 80)));
}

TEST_F(HorizontalLayoutTest, Paint_layout_with_one_view)
{
    layout.Add(view1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());

    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 11))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(20, 10)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(26, 14), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(26, 32767), layout.MaximumSize());

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(100, 80)));
}

TEST_F(HorizontalLayoutTest, Paint_layout_with_two_views)
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
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(69, 31))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(20, 10)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(29, 1), infra::Vector(40, 30)), view2.ViewRegion());

    EXPECT_EQ(infra::Vector(71, 34), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(71, 32767), layout.MaximumSize());

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    EXPECT_CALL(view2, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(100, 80)));
}

TEST_F(HorizontalLayoutTest, add_centre_aligned_view)
{
    layout.Add(view1, 0, services::VerticalAlignment::centre);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 44))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 34), infra::Vector(20, 10)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(26, 14), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(26, 32767), layout.MaximumSize());
}

TEST_F(HorizontalLayoutTest, add_bottom_aligned_view)
{
    layout.Add(view1, 0, services::VerticalAlignment::bottom);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 77))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 67), infra::Vector(20, 10)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(26, 14), layout.MinimumSize());

    EXPECT_CALL(view1, MaximumSize()).WillOnce(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_EQ(infra::Vector(26, 32767), layout.MaximumSize());
}

TEST_F(HorizontalLayoutTest, add_fill_aligned_view)
{
    layout.AddFill(view1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(5, 10)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 77))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(20, 76)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(26, 14), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(26, 1004), layout.MaximumSize());
}

TEST_F(HorizontalLayoutTest, add_two_proportional_views)
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
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(48, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(98, 31))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(44, 10)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(53, 1), infra::Vector(45, 30)), view2.ViewRegion());

    EXPECT_EQ(infra::Vector(11, 34), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(2011, 32767), layout.MaximumSize());
}

TEST_F(HorizontalLayoutTest, add_two_views_with_different_proportion)
{
    layout.Add(view1, 1);
    layout.Add(view2, 2);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(33, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(98, 31))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(29, 10)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(38, 1), infra::Vector(60, 30)), view2.ViewRegion());
}

TEST_F(HorizontalLayoutTest, add_two_proportional_views_with_minimum_size_restriction)
{
    layout.Add(view1, 1);
    layout.Add(view2, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(50, 0)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(43, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(98, 31))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(39, 10)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(48, 1), infra::Vector(50, 30)), view2.ViewRegion());
}

TEST_F(HorizontalLayoutTest, add_two_proportional_views_with_maximum_size_restriction)
{
    layout.Add(view1, 1);
    layout.Add(view2, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view2, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(30, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(view2, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(63, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(98, 31))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(59, 10)), view1.ViewRegion());
    EXPECT_EQ(infra::Region(infra::Point(68, 1), infra::Vector(30, 30)), view2.ViewRegion());
}

TEST_F(HorizontalLayoutTest, add_proportional_view_with_dummy)
{
    layout.Add(view1, 1);
    layout.AddDummy(1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(1000, 1000)));
    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(48, 11))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(44, 10)), view1.ViewRegion());

    EXPECT_EQ(infra::Vector(11, 14), layout.MinimumSize());
    EXPECT_EQ(infra::Vector(1011, 32767), layout.MaximumSize());
}

TEST_F(HorizontalLayoutTest, bring_view_to_front)
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
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(24, 11))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(69, 31))));
    layout.ResetLayout();

    testing::InSequence s;

    layout.BringToFront(view2);
    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    EXPECT_CALL(view2, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(100, 80))));
    layout.Paint(canvas, infra::Region(infra::Point(), infra::Vector(100, 80)));
}

TEST_F(HorizontalLayoutTest, MinimumSize_is_respected)
{
    layout.AddFill(view1, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(100, 100)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));

    EXPECT_EQ(infra::Vector(106, 104), layout.MinimumSize());

    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(104, 101))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(100, 100)), view1.ViewRegion());
}

TEST_F(HorizontalLayoutTest, MaximumSize_is_respected)
{
    layout.AddFill(view1, 1);
    EXPECT_CALL(view1, ResetSize());
    EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(0, 0)));
    EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(10, 10)));

    EXPECT_EQ(infra::Vector(16, 14), layout.MaximumSize());

    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(20, 11))));
    layout.ResetLayout();
    EXPECT_EQ(infra::Region(infra::Point(4, 1), infra::Vector(10, 10)), view1.ViewRegion());
}
