#include "preview/touch/TouchViewMultiple.hpp"
#include "infra/util/ReverseRange.hpp"

namespace services
{
    TouchViewMultiple::TouchViewMultiple(infra::BoundedVector<View*>& views, infra::BoundedVector<TouchRecipient*>& touchRecipients)
        : TouchRecipient::WithView<ViewMultiple>(views)
        , touchRecipients(touchRecipients)
    {}

    void TouchViewMultiple::Add(TouchRecipient& view)
    {
        GetView().Add(view.GetView());
        touchRecipients.push_back(&view);
    }

    void TouchViewMultiple::Remove(TouchRecipient& view)
    {
        GetView().Remove(view.GetView());
        touchRecipients.erase(std::find(touchRecipients.begin(), touchRecipients.end(), &view));
    }

    void TouchViewMultiple::BringToFront(TouchRecipient& view)
    {
        GetView().BringToFront(view.GetView());

        auto iterator = std::find(touchRecipients.begin(), touchRecipients.end(), &view);
        assert(iterator != touchRecipients.end());

        touchRecipients.erase(iterator);
        touchRecipients.push_back(&view);
    }

    void TouchViewMultiple::StartTouch(infra::Point point)
    {
        assert(beingTouched == nullptr);

        for (auto recipient : infra::MakeReverseRange(touchRecipients))
        {
            if (recipient->GetView().ViewRegion().Contains(point))
            {
                beingTouched = recipient;
                BringToFront(*beingTouched);
                beingTouched->StartTouch(point);
                return;
            }
        }
    }

    void TouchViewMultiple::DragIn(infra::Point point)
    {
        assert(beingTouched == nullptr);

        for (auto recipient : infra::MakeReverseRange(touchRecipients))
        {
            if (recipient->GetView().ViewRegion().Contains(point))
            {
                beingTouched = recipient;
                BringToFront(*beingTouched);
                beingTouched->DragIn(point);
                return;
            }
        }
    }

    void TouchViewMultiple::DragTo(infra::Point point)
    {
        for (auto recipient : infra::MakeReverseRange(touchRecipients))
        {
            if (recipient->GetView().ViewRegion().Contains(point))
            {
                if (beingTouched == recipient)
                    beingTouched->DragTo(point);
                else
                {
                    DragOut();

                    beingTouched = recipient;
                    BringToFront(*beingTouched);
                    beingTouched->DragIn(point);
                }

                return;
            }
        }

        DragOut();
    }

    void TouchViewMultiple::DragOut()
    {
        if (beingTouched != nullptr)
        {
            beingTouched->DragOut();
            beingTouched = nullptr;
        }
    }

    void TouchViewMultiple::StopTouch()
    {
        if (beingTouched != nullptr)
        {
            beingTouched->StopTouch();
            beingTouched = nullptr;
        }
    }

    void TouchViewMultiple::Swipe(Direction direction)
    {
        if (beingTouched != nullptr)
        {
            beingTouched->Swipe(direction);
            beingTouched = nullptr;
        }
    }
}
