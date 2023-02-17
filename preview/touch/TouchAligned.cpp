#include "preview/touch/TouchAligned.hpp"

namespace services
{
    TouchAligned::TouchAligned(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
        : TouchRecipient::WithView<ViewAligned>(horizontalAlignment, verticalAlignment)
    {}

    void TouchAligned::StartTouch(infra::Point point)
    {
        if (subView != nullptr)
            subView->StartTouch(point);
    }

    void TouchAligned::DragIn(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragIn(point);
    }

    void TouchAligned::DragTo(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragTo(point);
    }

    void TouchAligned::DragOut()
    {
        if (subView != nullptr)
            subView->DragOut();
    }

    void TouchAligned::StopTouch()
    {
        if (subView != nullptr)
            subView->StopTouch();
    }

    void TouchAligned::Swipe(Direction direction)
    {
        if (subView != nullptr)
            subView->Swipe(direction);
    }

    void TouchAligned::SetSubView(TouchRecipient& newSubView)
    {
        subView = &newSubView;
        GetView().SetSubView(newSubView.GetView());
    }
}
