#include "gmock/gmock.h"
#include "preview/touch/TouchHorizontalLayout.hpp"
#include "preview/touch/test/TouchRecipientMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

class TouchHorizontalLayoutTest
    : public testing::Test
{
public:
    TouchHorizontalLayoutTest()
        : view1(infra::Vector(10, 20))
        , view2(infra::Vector(10, 20))
    {
        horizontalLayout.Add(view1);
        horizontalLayout.AddDummy(1);
        horizontalLayout.Add(view2);

        EXPECT_CALL(view1.GetView(), MinimumSize()).WillRepeatedly(testing::Return(infra::Vector()));
        EXPECT_CALL(view1.GetView(), MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
        EXPECT_CALL(view2.GetView(), MinimumSize()).WillRepeatedly(testing::Return(infra::Vector()));
        EXPECT_CALL(view2.GetView(), MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
        EXPECT_CALL(view2.GetView(), ViewRegionChanged());
        horizontalLayout.GetView().Resize(infra::Vector(30, 20));
    }

    services::TouchHorizontalLayout::WithMaxViews<5> horizontalLayout;
    testing::StrictMock<services::TouchRecipientMock> view1;
    testing::StrictMock<services::TouchRecipientMock> view2;
};

TEST_F(TouchHorizontalLayoutTest, StartTouch_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));
}

TEST_F(TouchHorizontalLayoutTest, DragIn_in_second_view)
{
    EXPECT_CALL(view2, DragIn(infra::Point(20, 0)));
    horizontalLayout.DragIn(infra::Point(20, 0));
}

TEST_F(TouchHorizontalLayoutTest, DragTo_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));

    EXPECT_CALL(view2, DragTo(infra::Point(20, 5)));
    horizontalLayout.DragTo(infra::Point(20, 5));
}

TEST_F(TouchHorizontalLayoutTest, DragOut_of_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));

    EXPECT_CALL(view2, DragOut());
    horizontalLayout.DragTo(infra::Point(15, 0));
}

TEST_F(TouchHorizontalLayoutTest, DragIn_to_first_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));

    EXPECT_CALL(view2, DragOut());
    EXPECT_CALL(view1, DragIn(infra::Point(5, 0)));
    horizontalLayout.DragTo(infra::Point(5, 0));
}

TEST_F(TouchHorizontalLayoutTest, StopTouch_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));

    EXPECT_CALL(view2, StopTouch());
    horizontalLayout.StopTouch();
}

TEST_F(TouchHorizontalLayoutTest, Swipe_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(20, 0)));
    horizontalLayout.StartTouch(infra::Point(20, 0));

    EXPECT_CALL(view2, Swipe(services::Direction::left));
    horizontalLayout.Swipe(services::Direction::left);
}

TEST_F(TouchHorizontalLayoutTest, in_overlapping_views_last_view_gets_priority)
{
    EXPECT_CALL(view2.GetView(), ViewRegionChanged());
    view2.GetView().Move(infra::Point());

    EXPECT_CALL(view2, StartTouch(infra::Point()));
    horizontalLayout.StartTouch(infra::Point());
}

TEST_F(TouchHorizontalLayoutTest, BringToFront_gives_priority_to_fist_view)
{
    EXPECT_CALL(view2.GetView(), ViewRegionChanged());
    view2.GetView().Move(infra::Point());

    horizontalLayout.BringToFront(view1);
    EXPECT_CALL(view1, StartTouch(infra::Point()));
    horizontalLayout.StartTouch(infra::Point());
}
