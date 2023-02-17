#ifndef PREVIEW_TOUCH_ALIGNED_HPP
#define PREVIEW_TOUCH_ALIGNED_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/ViewAligned.hpp"

namespace services
{
    class TouchAligned
        : public TouchRecipient::WithView<ViewAligned>
    {
    public:
        template<class View>
        class WithView;

        TouchAligned(HorizontalAlignment horizontalAlignment = HorizontalAlignment::centre, VerticalAlignment verticalAlignment = VerticalAlignment::centre);

        virtual void StartTouch(infra::Point point) override;
        virtual void DragIn(infra::Point point) override;
        virtual void DragTo(infra::Point point) override;
        virtual void DragOut() override;
        virtual void StopTouch() override;
        virtual void Swipe(Direction direction) override;

        void SetSubView(TouchRecipient& newSubView);

    private:
        TouchRecipient* subView;
    };

    template<class T>
    class TouchAligned::WithView
        : public TouchAligned
    {
    public:
        template<class... Args>
        WithView(Args&&... args)
            : subView(std::forward<Args>(args)...)
        {
            SetSubView(subView);
        }

        template<class... Args>
        WithView(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, Args&&... args)
            : TouchAligned(horizontalAlignment, verticalAlignment)
            , subView(std::forward<Args>(args)...)
        {
            SetSubView(subView);
        }

        T& SubView()
        {
            return subView;
        }

    private:
        T subView;
    };
}

#endif
