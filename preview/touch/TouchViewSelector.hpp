#ifndef PREVIEW_TOUCH_VIEW_SELECTOR_HPP
#define PREVIEW_TOUCH_VIEW_SELECTOR_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/ViewSelector.hpp"

namespace services
{
    class TouchViewSelector
        : public TouchRecipient
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<infra::WithStorage<TouchViewSelector, ViewSelector::WithMaxViews<N>>, infra::BoundedVector<TouchRecipient*>::WithMaxSize<N>>;

        TouchViewSelector(ViewSelector& viewSelector, infra::BoundedVector<TouchRecipient*>& touchRecipients, View& topView);

        virtual void StartTouch(infra::Point point) override;
        virtual void DragIn(infra::Point point) override;
        virtual void DragTo(infra::Point point) override;
        virtual void DragOut() override;
        virtual void StopTouch() override;
        virtual void Swipe(Direction direction) override;

        virtual ViewSelector& GetView() override;
        void Add(TouchRecipient& view);
        void Remove(TouchRecipient& view);

    private:
        void SearchCurrentTouchRecipient();

    private:
        ViewSelector& viewSelector;
        infra::BoundedVector<TouchRecipient*>& touchRecipients;
        View& topView;
        TouchRecipient* selectedTouchRecipient = nullptr;
    };
}

#endif
