#ifndef PREVIEW_TOUCH_VERTICAL_LAYOUT_HPP
#define PREVIEW_TOUCH_VERTICAL_LAYOUT_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/VerticalLayout.hpp"

namespace services
{
    class TouchVerticalLayout
        : public TouchRecipient::WithView<VerticalLayout>
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<infra::WithStorage<TouchVerticalLayout, infra::BoundedVector<detail::LayoutViewInfo>::WithMaxSize<N>>, infra::BoundedVector<TouchRecipient*>::WithMaxSize<N>>;

        TouchVerticalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, infra::BoundedVector<TouchRecipient*>& touchRecipients, uint8_t interMargin = 0, uint8_t topMargin = 0, uint8_t rightMargin = 0, uint8_t bottomMargin = 0, uint8_t leftMargin = 0);

        void Add(TouchRecipient& view, uint16_t proportion = 0, HorizontalAlignment alignment = HorizontalAlignment::left);
        void AddFill(TouchRecipient& view, uint16_t proportion = 0);
        void AddDummy(uint16_t proportion);

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
