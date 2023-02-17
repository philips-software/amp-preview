#ifndef PREVIEW_VIEW_BUTTON_PANEL_HPP
#define PREVIEW_VIEW_BUTTON_PANEL_HPP

#include "preview/interfaces/Button.hpp"
#include "preview/views/ViewPanel.hpp"

namespace services
{
    class ViewButtonPanel
        : public ViewPanel
        , public Button
    {
    public:
        template<class T>
        class WithView;

        ViewButtonPanel(infra::Colour inactiveColour, infra::Colour activeColour);

        // Implementation of Button
        virtual void Activate() override;
        virtual void Deactivate() override;

    private:
        infra::Colour inactiveColour;
        infra::Colour activeColour;
    };

    template<class T>
    class ViewButtonPanel::WithView
        : public ViewButtonPanel
    {
    public:
        template<class... Args>
        WithView(infra::Colour inactiveColour, infra::Colour activeColour, Args&&... args)
            : ViewButtonPanel(inactiveColour, activeColour)
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
