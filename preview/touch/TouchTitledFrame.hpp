#ifndef PREVIEW_TOUCH_TITLED_FRAME_HPP
#define PREVIEW_TOUCH_TITLED_FRAME_HPP

#include "preview/touch/TouchSubView.hpp"
#include "preview/views/ViewTitledFrame.hpp"

namespace services
{
    class TouchTitledFrame
    {
    public:
        template<class T>
        class WithTouch;
    };

    template<class T>
    class TouchTitledFrame::WithTouch
        : public TouchSubView::WithTouch<ViewTitledFrame>
    {
    public:
        template<class... Args>
        WithTouch(TitledFrameAttributes attributes, infra::BoundedConstString text, Args&&... args);

        T& SubView();

    private:
        T subView;
    };

    ////    Implementation    ////

    template<class T>
    template<class... Args>
    TouchTitledFrame::WithTouch<T>::WithTouch(TitledFrameAttributes attributes, infra::BoundedConstString text, Args&&... args)
        : TouchSubView::WithTouch<ViewTitledFrame>(attributes, text)
        , subView(std::forward<Args>(args)...)
    {
        SetSubView(subView);
    }

    template<class T>
    T& TouchTitledFrame::WithTouch<T>::SubView()
    {
        return subView;
    }
}

#endif
