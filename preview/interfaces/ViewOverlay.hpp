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

        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

    protected:
        void ViewRegionChanged() override;

    private:
        View& underView;
    };
}

#endif
