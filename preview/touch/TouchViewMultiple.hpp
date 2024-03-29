#ifndef PREVIEW_TOUCH_VIEW_MULTIPLE_HPP
#define PREVIEW_TOUCH_VIEW_MULTIPLE_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/ViewMultiple.hpp"

namespace services
{
    class TouchViewMultiple
        : public TouchRecipient::WithView<ViewMultiple>
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<infra::WithStorage<TouchViewMultiple, infra::BoundedVector<View*>::WithMaxSize<N>>, infra::BoundedVector<TouchRecipient*>::WithMaxSize<N>>;

        TouchViewMultiple(infra::BoundedVector<View*>& views, infra::BoundedVector<TouchRecipient*>& touchRecipients);

        void Add(TouchRecipient& view);
        void Remove(TouchRecipient& view);
        void BringToFront(TouchRecipient& view);

        // Implementation of TouchRecipient
        void StartTouch(infra::Point point) override;
        void DragIn(infra::Point point) override;
        void DragTo(infra::Point point) override;
        void DragOut() override;
        void StopTouch() override;
        void Swipe(Direction direction) override;

    private:
        infra::BoundedVector<TouchRecipient*>& touchRecipients;
        TouchRecipient* beingTouched = nullptr;
    };
}

#endif
