#ifndef PREVIEW_VIEW_FRAME_HPP
#define PREVIEW_VIEW_FRAME_HPP

#include "preview/views/ViewPanel.hpp"

namespace services
{
    struct FrameAttributes
    {
        infra::Colour frameColour;
        infra::Colour backgroundColour;
    };

    class ViewFrame
        : public View
    {
    public:
        template<class T>
        class WithView;

        ViewFrame(FrameAttributes attributes);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetFrameColour(infra::Colour newColour);
        void SetBackgroundColour(infra::Colour newColour);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        infra::Colour frameColour;
        ViewPanel panel;
    };

    template<class T>
    class ViewFrame::WithView
        : public ViewFrame
    {
    public:
        template<class... Args>
        WithView(FrameAttributes attributes, Args&&... args)
            : ViewFrame(attributes)
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
