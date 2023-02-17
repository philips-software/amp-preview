#include "preview/touch/TouchSubView.hpp"

namespace services
{
    void TouchSubView::StartTouch(infra::Point point)
    {
        if (subView != nullptr)
            subView->StartTouch(point);
    }

    void TouchSubView::DragIn(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragIn(point);
    }

    void TouchSubView::DragTo(infra::Point point)
    {
        if (subView != nullptr)
            subView->DragTo(point);
    }

    void TouchSubView::DragOut()
    {
        if (subView != nullptr)
            subView->DragOut();
    }

    void TouchSubView::StopTouch()
    {
        if (subView != nullptr)
            subView->StopTouch();
    }

    void TouchSubView::Swipe(Direction direction)
    {
        if (subView != nullptr)
            subView->Swipe(direction);
    }

    void TouchSubView::SetSubView(TouchRecipient& newSubView)
    {
        subView = &newSubView;
    }
}
