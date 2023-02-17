#include "preview/views/HorizontalShifter.hpp"

namespace services
{
    HorizontalShifter::HorizontalShifter(View& subView)
        : subView(subView)
    {
        Resize(infra::Vector(0, subView.ViewRegion().Size().deltaY));
    }

    void HorizontalShifter::SetShift(int16_t newShift)
    {
        shift = newShift;
        ResetLayout();
    }

    infra::Vector HorizontalShifter::MinimumSize() const
    {
        return subView.MinimumSize();
    }

    infra::Vector HorizontalShifter::MaximumSize() const
    {
        return subView.MaximumSize();
    }

    void HorizontalShifter::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        subView.Paint(canvas, boundingRegion);
    }

    void HorizontalShifter::ResetSize()
    {
        subView.ResetSize();
    }

    void HorizontalShifter::ViewRegionChanged()
    {
        subView.SetViewRegion(infra::Region(ViewRegion().Top(), ViewRegion().Left() + shift, ViewRegion().Bottom(), ViewRegion().Left() + shift + subView.ViewRegion().Width()));
    }
}
