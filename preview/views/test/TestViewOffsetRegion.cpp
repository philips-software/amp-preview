#include "gmock/gmock.h"
#include "preview/views/ViewOffsetRegion.hpp"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

TEST(ViewOffsetRegionWithoutSubViewTest, nothing_really_happens)
{
    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewOffsetRegion view(infra::RegionOffset({ 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }));
    testing::StrictMock<services::TopViewMock> topView{ view };

    EXPECT_EQ(infra::Vector(), view.MinimumSize());
    EXPECT_EQ(infra::Vector(), view.MaximumSize());
    view.Paint(canvas, infra::Region());
    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.SetViewRegion(infra::Region());
}

class ViewOffsetRegionTest
    : public testing::Test
{
public:
    ViewOffsetRegionTest()
        : view(infra::RegionOffset({ -1, 0 }, { 0, 0 }, { 0, 0 }, { 1, 0 }), infra::Vector())
    {}

    testing::StrictMock<hal::CanvasMock> canvas;
    services::ViewOffsetRegion::WithView<testing::StrictMock<services::ViewMock>> view;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(ViewOffsetRegionTest, MinimumSize_is_adjusted)
{
    EXPECT_CALL(view.SubView(), MinimumSize()).WillOnce(testing::Return(infra::Vector(100, 200)));
    EXPECT_EQ(infra::Vector(200, 400), view.MinimumSize());
}

TEST_F(ViewOffsetRegionTest, MaximumSize_is_adjusted)
{
    EXPECT_CALL(view.SubView(), MaximumSize()).WillOnce(testing::Return(infra::Vector(100, 200)));
    EXPECT_EQ(infra::Vector(200, 400), view.MaximumSize());
}

TEST_F(ViewOffsetRegionTest, Paint_is_forwarded)
{
    EXPECT_CALL(view.SubView(), Paint(testing::Ref(canvas), infra::Region(1, 2, 3, 4)));
    view.Paint(canvas, infra::Region(1, 2, 3, 4));
}

TEST_F(ViewOffsetRegionTest, SetViewRegion_is_translated)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(2, 1), infra::Vector(2, 2))));
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(2, -1), infra::Vector(4, 4))));
    view.SetViewRegion(infra::Region(1, 2, 3, 4));
    EXPECT_EQ(infra::Region(-1, 2, 3, 6), view.SubView().ViewRegion());
}

TEST_F(ViewOffsetRegionTest, ResetSize_resets_subview_size)
{
    EXPECT_CALL(view.SubView(), ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(150, 200))));
    view.SubView().Resize(infra::Vector(150, 200));

    EXPECT_CALL(view.SubView(), ResetSize());
    view.ResetSize();

    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(300, 400)), view.ViewRegion());
}
