#include "preview/touch/TouchViewSelector.hpp"

namespace services
{
    TouchViewSelector::TouchViewSelector(ViewSelector& viewSelector, infra::BoundedVector<TouchRecipient*>& touchRecipients, View& topView)
        : viewSelector(viewSelector)
        , touchRecipients(touchRecipients)
        , topView(topView)
    {}

    void TouchViewSelector::StartTouch(infra::Point point)
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->StartTouch(point);
    }

    void TouchViewSelector::DragIn(infra::Point point)
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->DragIn(point);
    }

    void TouchViewSelector::DragTo(infra::Point point)
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->DragTo(point);
    }

    void TouchViewSelector::DragOut()
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->DragOut();
    }

    void TouchViewSelector::StopTouch()
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->StopTouch();
    }

    void TouchViewSelector::Swipe(Direction direction)
    {
        if (selectedTouchRecipient != nullptr)
            selectedTouchRecipient->StopTouch();

        if (direction == Direction::right)
        {
            viewSelector.Next();
            topView.ResetLayout();

            SearchCurrentTouchRecipient();
        }
        else if (direction == Direction::left)
        {
            viewSelector.Previous();
            topView.ResetLayout();

            SearchCurrentTouchRecipient();
        }
    }

    ViewSelector& TouchViewSelector::GetView()
    {
        return viewSelector;
    }

    void TouchViewSelector::Add(TouchRecipient& view)
    {
        touchRecipients.push_back(&view);
        viewSelector.Add(view.GetView());
        SearchCurrentTouchRecipient();
    }

    void TouchViewSelector::Remove(TouchRecipient& view)
    {
        touchRecipients.erase(std::find(touchRecipients.begin(), touchRecipients.end(), &view));
        viewSelector.Remove(view.GetView());
        SearchCurrentTouchRecipient();
    }

    void TouchViewSelector::SearchCurrentTouchRecipient()
    {
        for (auto touchRecipient : touchRecipients)
            if (&touchRecipient->GetView() == &viewSelector.Current())
            {
                selectedTouchRecipient = touchRecipient;
                return;
            }

        selectedTouchRecipient = nullptr;
    }
}
