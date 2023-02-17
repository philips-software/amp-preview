#ifndef PREVIEW_VIEW_OVERLAY_HPP
#define PREVIEW_VIEW_OVERLAY_HPP

#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewOverlay
        : public View
    {
    public:
        ViewOverlay(View& underView);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

    protected:
        virtual void ViewRegionChanged() override;

    private:
        View& underView;
    };
}

#endif
