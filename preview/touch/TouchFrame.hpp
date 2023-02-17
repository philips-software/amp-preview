#ifndef PREVIEW_TOUCH_FRAME_HPP
#define PREVIEW_TOUCH_FRAME_HPP

#include "preview/touch/TouchSubView.hpp"
#include "preview/views/ViewFrame.hpp"

namespace services
{
    class TouchFrame
    {
    public:
        template<class T>
        class WithTouch;
    };

    template<class T>
    class TouchFrame::WithTouch
        : public TouchSubView::WithTouch<ViewFrame>
    {
    public:
        template<class... Args>
        WithTouch(FrameAttributes attributes, Args&&... args);

        T& SubView();

    private:
        T subView;
    };

    ////    Implementation    ////

    template<class T>
    template<class... Args>
    TouchFrame::WithTouch<T>::WithTouch(FrameAttributes attributes, Args&&... args)
        : TouchSubView::WithTouch<ViewFrame>(attributes)
        , subView(std::forward<Args>(args)...)
    {
        SetSubView(subView);
    }

    template<class T>
    T& TouchFrame::WithTouch<T>::SubView()
    {
        return subView;
    }
}

#endif
