#ifndef PREVIEW_TOUCH_SUB_VIEW_HPP
#define PREVIEW_TOUCH_SUB_VIEW_HPP

#include "preview/touch/TouchRecipient.hpp"

namespace services
{
    class TouchSubView
        : public TouchRecipient
    {
    public:
        template<class T>
        class WithTouch;

        // Implementation of TouchRecipient
        void StartTouch(infra::Point point) override;
        void DragIn(infra::Point point) override;
        void DragTo(infra::Point point) override;
        void DragOut() override;
        void StopTouch() override;
        void Swipe(Direction direction) override;

        virtual void SetSubView(TouchRecipient& newSubView) = 0;

    private:
        TouchRecipient* subView;
    };

    template<class T>
    class TouchSubView::WithTouch
        : public TouchSubView
    {
    public:
        template<class... Args>
        WithTouch(Args&&... args);

        T& GetView() override;
        void SetSubView(TouchRecipient& newSubView) override;

    private:
        T view;
    };

    ////    Implementation    ////

    template<class T>
    template<class... Args>
    TouchSubView::WithTouch<T>::WithTouch(Args&&... args)
        : view(std::forward<Args>(args)...)
    {}

    template<class T>
    T& TouchSubView::WithTouch<T>::GetView()
    {
        return view;
    }

    template<class T>
    void TouchSubView::WithTouch<T>::SetSubView(TouchRecipient& newSubView)
    {
        TouchSubView::SetSubView(newSubView);
        GetView().SetSubView(newSubView.GetView());
    }
}

#endif
