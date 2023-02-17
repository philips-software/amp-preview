#ifndef PREVIEW_VIEW_TITLED_FRAME_HPP
#define PREVIEW_VIEW_TITLED_FRAME_HPP

#include "preview/views/ViewFrame.hpp"
#include "preview/views/ViewFramedText.hpp"

namespace services
{
    struct TitledFrameAttributes
    {
        TitledFrameAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour textColour = infra::Colour::white, infra::Font font = infra::freeSans12pt7b);

        infra::Colour frameColour;
        infra::Colour backgroundColour;
        infra::Colour textColour;
        infra::Font font;
    };

    class ViewTitledFrame
        : public View
    {
    public:
        template<class T>
        class WithView;

        ViewTitledFrame(TitledFrameAttributes attributes, infra::BoundedConstString text);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetFrameColour(infra::Colour newColour);
        void SetBackgroundColour(infra::Colour newColour);
        void SetTextColour(infra::Colour newColour);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        ViewFramedText text;
        ViewFrame frame;
    };

    template<class T>
    class ViewTitledFrame::WithView
        : public ViewTitledFrame
    {
    public:
        template<class... Args>
        WithView(TitledFrameAttributes attributes, infra::BoundedConstString text, Args&&... args)
            : ViewTitledFrame(attributes, text)
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
