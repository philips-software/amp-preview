#include "preview/touch/TouchButton.hpp"

namespace services
{
    TouchButton::TouchButton(View& view, Button& button, infra::Function<void()> onPressed)
        : view(view)
        , button(button)
        , onPressed(onPressed)
    {}

    void TouchButton::StartTouch(infra::Point point)
    {
        activated = true;
        button.Activate();
    }

    void TouchButton::DragIn(infra::Point point)
    {}

    void TouchButton::DragTo(infra::Point point)
    {}

    void TouchButton::DragOut()
    {
        if (activated)
            button.Deactivate();
    }

    void TouchButton::StopTouch()
    {
        if (activated)
        {
            button.Deactivate();
            onPressed();
        }
    }

    void TouchButton::Swipe(Direction direction)
    {
        if (activated)
            button.Deactivate();
    }

    View& TouchButton::GetView()
    {
        return view;
    }
}
