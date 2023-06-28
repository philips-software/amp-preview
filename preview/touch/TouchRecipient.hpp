#ifndef PREVIEW_TOUCH_RECIPIENT_HPP
#define PREVIEW_TOUCH_RECIPIENT_HPP

#include "preview/interfaces/View.hpp"

namespace services
{
    class TouchRecipient
    {
    public:
        template<class T>
        class WithView;

    protected:
        TouchRecipient() = default;
        TouchRecipient(const TouchRecipient& other) = delete;
        TouchRecipient& operator=(const TouchRecipient& other) = delete;
        ~TouchRecipient() = default;

    public:
        virtual void StartTouch(infra::Point point) = 0;
        virtual void DragIn(infra::Point point) = 0;
        virtual void DragTo(infra::Point point) = 0;
        virtual void DragOut() = 0;
        virtual void StopTouch() = 0;
        virtual void Swipe(Direction direction) = 0;

        virtual View& GetView() = 0;
    };

    template<class T>
    class TouchRecipient::WithView
        : public TouchRecipient
    {
    public:
        template<class... Args>
        WithView(Args&&... args);

        T& GetView() override;

    private:
        T view;
    };

    ////    Implementation    ////

    template<class T>
    template<class... Args>
    TouchRecipient::WithView<T>::WithView(Args&&... args)
        : view(std::forward<Args>(args)...)
    {}

    template<class T>
    T& TouchRecipient::WithView<T>::GetView()
    {
        return view;
    }
}

#endif
