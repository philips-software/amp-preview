#ifndef PREVIEW_TOUCH_PANEL_HPP
#define PREVIEW_TOUCH_PANEL_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/ViewPanel.hpp"

namespace services
{
    class TouchPanel
        : public TouchRecipient::WithView<ViewPanel>
    {
    public:
        template<class View>
        class WithView;

        TouchPanel(infra::Colour colour);

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
    class TouchPanel::WithView
        : public TouchPanel
    {
    public:
        template<class... Args>
        WithView(infra::Colour colour, Args&&... args)
            : TouchPanel(colour)
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
