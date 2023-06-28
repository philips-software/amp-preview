#include "preview/touch/TouchHorizontalLayout.hpp"
#include "infra/util/ReverseRange.hpp"

namespace services
{
    TouchHorizontalLayout::TouchHorizontalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, infra::BoundedVector<TouchRecipient*>& touchRecipients, uint8_t interMargin, uint8_t topMargin, uint8_t rightMargin, uint8_t bottomMargin, uint8_t leftMargin)
        : TouchRecipient::WithView<HorizontalLayout>(views, interMargin, topMargin, rightMargin, bottomMargin, leftMargin)
        , touchRecipients(touchRecipients)
    {}

    void TouchHorizontalLayout::Add(TouchRecipient& view, uint16_t proportion, VerticalAlignment alignment)
    {
        GetView().Add(view.GetView(), proportion, alignment);
        touchRecipients.push_back(&view);
    }

    void TouchHorizontalLayout::AddFill(TouchRecipient& view, uint16_t proportion)
    {
        GetView().AddFill(view.GetView(), proportion);
        touchRecipients.push_back(&view);
    }

    void TouchHorizontalLayout::AddDummy(uint16_t proportion)
    {
        GetView().AddDummy(proportion);
    }

    void TouchHorizontalLayout::BringToFront(TouchRecipient& view)
    {
        GetView().BringToFront(view.GetView());

        auto recipientIndex = std::find_if(touchRecipients.begin(), touchRecipients.end(), [&view](TouchRecipient* recipient)
            {
                return recipient == &view;
            });
        assert(recipientIndex != touchRecipients.end());

        TouchRecipient* recipient = *recipientIndex;
        touchRecipients.erase(recipientIndex);
        touchRecipients.push_back(recipient);
    }

    void TouchHorizontalLayout::StartTouch(infra::Point point)
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

    void TouchHorizontalLayout::DragIn(infra::Point point)
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

    void TouchHorizontalLayout::DragTo(infra::Point point)
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

    void TouchHorizontalLayout::DragOut()
    {
        if (beingTouched != nullptr)
        {
            beingTouched->DragOut();
            beingTouched = nullptr;
        }
    }

    void TouchHorizontalLayout::StopTouch()
    {
        if (beingTouched != nullptr)
        {
            beingTouched->StopTouch();
            beingTouched = nullptr;
        }
    }

    void TouchHorizontalLayout::Swipe(Direction direction)
    {
        if (beingTouched != nullptr)
        {
            beingTouched->Swipe(direction);
            beingTouched = nullptr;
        }
    }
}
