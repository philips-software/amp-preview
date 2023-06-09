#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/touch/TouchVerticalLayout.hpp"
#include "preview/touch/test/TouchRecipientMock.hpp"
#include "gmock/gmock.h"

class TouchVerticalLayoutTest
    : public testing::Test
{
public:
    TouchVerticalLayoutTest()
        : view1(infra::Vector(20, 10))
        , view2(infra::Vector(20, 10))
    {
        verticalLayout.Add(view1);
        verticalLayout.AddDummy(1);
        verticalLayout.Add(view2);

        EXPECT_CALL(view1.GetView(), MinimumSize()).WillRepeatedly(testing::Return(infra::Vector()));
        EXPECT_CALL(view1.GetView(), MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
        EXPECT_CALL(view2.GetView(), MinimumSize()).WillRepeatedly(testing::Return(infra::Vector()));
        EXPECT_CALL(view2.GetView(), MaximumSize()).WillRepeatedly(testing::Return(infra::Vector::Maximum()));
        EXPECT_CALL(view2.GetView(), ViewRegionChanged());
        verticalLayout.GetView().Resize(infra::Vector(20, 30));
    }

    services::TouchVerticalLayout::WithMaxViews<5> verticalLayout;
    testing::StrictMock<services::TouchRecipientMock> view1;
    testing::StrictMock<services::TouchRecipientMock> view2;
};

TEST_F(TouchVerticalLayoutTest, StartTouch_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));
}

TEST_F(TouchVerticalLayoutTest, DragIn_in_second_view)
{
    EXPECT_CALL(view2, DragIn(infra::Point(0, 20)));
    verticalLayout.DragIn(infra::Point(0, 20));
}

TEST_F(TouchVerticalLayoutTest, DragTo_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));

    EXPECT_CALL(view2, DragTo(infra::Point(5, 20)));
    verticalLayout.DragTo(infra::Point(5, 20));
}

TEST_F(TouchVerticalLayoutTest, DragOut_of_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));

    EXPECT_CALL(view2, DragOut());
    verticalLayout.DragTo(infra::Point(0, 15));
}

TEST_F(TouchVerticalLayoutTest, DragIn_to_first_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));

    EXPECT_CALL(view2, DragOut());
    EXPECT_CALL(view1, DragIn(infra::Point(0, 5)));
    verticalLayout.DragTo(infra::Point(0, 5));
}

TEST_F(TouchVerticalLayoutTest, StopTouch_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));

    EXPECT_CALL(view2, StopTouch());
    verticalLayout.StopTouch();
}

TEST_F(TouchVerticalLayoutTest, Swipe_in_second_view)
{
    EXPECT_CALL(view2, StartTouch(infra::Point(0, 20)));
    verticalLayout.StartTouch(infra::Point(0, 20));

    EXPECT_CALL(view2, Swipe(services::Direction::left));
    verticalLayout.Swipe(services::Direction::left);
}

TEST_F(TouchVerticalLayoutTest, in_overlapping_views_last_view_gets_priority)
{
    EXPECT_CALL(view2.GetView(), ViewRegionChanged());
    view2.GetView().Move(infra::Point());

    EXPECT_CALL(view2, StartTouch(infra::Point()));
    verticalLayout.StartTouch(infra::Point());
}

TEST_F(TouchVerticalLayoutTest, BringToFront_gives_priority_to_fist_view)
{
    EXPECT_CALL(view2.GetView(), ViewRegionChanged());
    view2.GetView().Move(infra::Point());

    verticalLayout.BringToFront(view1);
    EXPECT_CALL(view1, StartTouch(infra::Point()));
    verticalLayout.StartTouch(infra::Point());
}
