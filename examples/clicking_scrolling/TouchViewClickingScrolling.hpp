#ifndef EXAMPLES_CLICKING_SCROLLING_TOUCH_VIEW_CLICKING_SCROLLING_HPP
#define EXAMPLES_CLICKING_SCROLLING_TOUCH_VIEW_CLICKING_SCROLLING_HPP

#include "preview/touch/TouchButton.hpp"
#include "preview/touch/TouchPanel.hpp"
#include "preview/touch/TouchVerticalLayout.hpp"
#include "preview/views/ViewFramedTextButton.hpp"
#include "preview/views/ViewScrolling.hpp"
#include "preview/views/ViewText.hpp"

namespace application
{
    class TouchViewClickingScrolling
        : public services::TouchPanel
    {
    public:
        TouchViewClickingScrolling();

    private:
        void OnTouched();

    private:
        services::TouchVerticalLayout::WithMaxViews<5> layout;
        services::ViewScrolling::WithView<services::ViewText> scrollingText;
        services::TouchButton::With<services::ViewFramedTextButton> button;
    };
}

#endif
