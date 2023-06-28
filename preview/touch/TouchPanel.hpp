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

        explicit TouchPanel(infra::Colour colour);

        void StartTouch(infra::Point point) override;
        void DragIn(infra::Point point) override;
        void DragTo(infra::Point point) override;
        void DragOut() override;
        void StopTouch() override;
        void Swipe(Direction direction) override;

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
