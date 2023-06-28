#ifndef PREVIEW_TOUCH_BUTTON_HPP
#define PREVIEW_TOUCH_BUTTON_HPP

#include "infra/util/Function.hpp"
#include "preview/interfaces/Button.hpp"
#include "preview/touch/TouchRecipient.hpp"

namespace services
{
    class TouchButton
        : public TouchRecipient
    {
    public:
        template<class T>
        class With;

        TouchButton(View& view, Button& button, infra::Function<void()> onPressed);

        void StartTouch(infra::Point point) override;
        void DragIn(infra::Point point) override;
        void DragTo(infra::Point point) override;
        void DragOut() override;
        void StopTouch() override;
        void Swipe(Direction direction) override;
        View& GetView() override;

    private:
        View& view;
        Button& button;
        bool activated = false;
        infra::Function<void()> onPressed;
    };

    template<class T>
    class TouchButton::With
        : public TouchButton
    {
    public:
        template<class... Args>
        With(infra::Function<void()> onPressed, Args&&... args);

        T& ViewButton();

    private:
        T viewButton;
    };

    ////    Implementation    ////

    template<class T>
    template<class... Args>
    TouchButton::With<T>::With(infra::Function<void()> onPressed, Args&&... args)
        : TouchButton(viewButton, viewButton, onPressed)
        , viewButton(std::forward<Args>(args)...)
    {}

    template<class T>
    T& TouchButton::With<T>::ViewButton()
    {
        return viewButton;
    }
}

#endif
