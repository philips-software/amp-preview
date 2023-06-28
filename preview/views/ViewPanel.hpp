#ifndef PREVIEW_VIEW_PANEL_HPP
#define PREVIEW_VIEW_PANEL_HPP

#include "preview/interfaces/Colour.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewPanel
        : public View
    {
    public:
        template<class T>
        class WithView;

        ViewPanel(infra::Colour colour);

        // Implementation of View
        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetColour(infra::Colour newColour);

    protected:
        void ViewRegionChanged() override;

    private:
        services::View* subView = nullptr;
        infra::Colour colour;
    };

    template<class T>
    class ViewPanel::WithView
        : public ViewPanel
    {
    public:
        template<class... Args>
        WithView(infra::Colour colour, Args&&... args)
            : ViewPanel(colour)
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
