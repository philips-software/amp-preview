#include "infra/util/test_helper/MockCallback.hpp"
#include "preview/interfaces/test_doubles/ButtonMock.hpp"
#include "preview/touch/TouchButton.hpp"
#include "gmock/gmock.h"

class TouchButtonTest
    : public testing::Test
{
public:
    TouchButtonTest()
        : touchButton([this]()
              {
                  callback.callback();
              },
              infra::Vector(1, 2))
    {}

    testing::StrictMock<infra::MockCallback<void()>> callback;
    services::TouchButton::With<testing::StrictMock<services::ViewButtonMock>> touchButton;
};

TEST_F(TouchButtonTest, StartPress_activates_button)
{
    EXPECT_CALL(touchButton.ViewButton(), Activate());
    touchButton.StartTouch(infra::Point());
}

TEST_F(TouchButtonTest, DragIn_does_not_activate_button)
{
    touchButton.DragIn(infra::Point());
}

TEST_F(TouchButtonTest, StopPress_deactivates_button_and_triggers_callback)
{
    // build
    EXPECT_CALL(touchButton.ViewButton(), Activate());
    touchButton.StartTouch(infra::Point());

    // operate/check
    EXPECT_CALL(touchButton.ViewButton(), Deactivate());
    EXPECT_CALL(callback, callback());
    touchButton.StopTouch();
}

TEST_F(TouchButtonTest, DragOut_deactivates_button)
{
    // build
    EXPECT_CALL(touchButton.ViewButton(), Activate());
    touchButton.StartTouch(infra::Point());

    // operate/check
    EXPECT_CALL(touchButton.ViewButton(), Deactivate());
    touchButton.DragOut();
}

TEST_F(TouchButtonTest, DragTo_does_nothing)
{
    // build
    EXPECT_CALL(touchButton.ViewButton(), Activate());
    touchButton.StartTouch(infra::Point());

    // operate/check
    touchButton.DragTo(infra::Point());
}

TEST_F(TouchButtonTest, Swipe_deactivates_button)
{
    // build
    EXPECT_CALL(touchButton.ViewButton(), Activate());
    touchButton.StartTouch(infra::Point());

    // operate/check
    EXPECT_CALL(touchButton.ViewButton(), Deactivate());
    touchButton.Swipe(services::Direction::left);
}

TEST_F(TouchButtonTest, ViewRegion_returns_region_of_view)
{
    EXPECT_EQ(infra::Region(0, 0, 2, 1), touchButton.GetView().ViewRegion());
}
