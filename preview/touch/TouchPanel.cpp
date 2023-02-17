#include "preview/touch/TouchPanel.hpp"

namespace services
{
    TouchPanel::TouchPanel(infra::Colour colour)
        : TouchRecipient::WithView<ViewPanel>(colour)
    {}

    void TouchPanel::StartTouch(infra::Point point)
    {
        if (subView != nullptr)
            subView->StartTouch(point);
    }

    void TouchPanel::DragIn(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragIn(point);
    }

    void TouchPanel::DragTo(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragTo(point);
    }

    void TouchPanel::DragOut()
    {
        if (subView != nullptr)
            subView->DragOut();
    }

    void TouchPanel::StopTouch()
    {
        if (subView != nullptr)
            subView->StopTouch();
    }

    void TouchPanel::Swipe(Direction direction)
    {
        if (subView != nullptr)
            subView->Swipe(direction);
    }

    void TouchPanel::SetSubView(TouchRecipient& newSubView)
    {
        subView = &newSubView;
        GetView().SetSubView(newSubView.GetView());
    }
}
