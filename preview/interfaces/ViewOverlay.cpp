#include "preview/interfaces/ViewOverlay.hpp"

namespace services
{
    ViewOverlay::ViewOverlay(View& underView)
        : underView(underView)
    {
        underView.SetParent(*this);
    }

    infra::Vector ViewOverlay::MinimumSize() const
    {
        return underView.MinimumSize();
    }

    infra::Vector ViewOverlay::MaximumSize() const
    {
        return underView.MaximumSize();
    }

    void ViewOverlay::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        underView.Paint(canvas, boundingRegion);
    }

    void ViewOverlay::ViewRegionChanged()
    {
        underView.SetViewRegion(ViewRegion());
    }
}
