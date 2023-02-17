#include "preview/views/VerticalShifter.hpp"

namespace services
{
    VerticalShifter::VerticalShifter(View& subView)
        : subView(subView)
    {
        Resize(infra::Vector(subView.ViewRegion().Size().deltaX, 0));
    }

    void VerticalShifter::SetShift(int16_t newShift)
    {
        shift = newShift;
        ResetLayout();
    }

    infra::Vector VerticalShifter::MinimumSize() const
    {
        return subView.MinimumSize();
    }

    infra::Vector VerticalShifter::MaximumSize() const
    {
        return subView.MaximumSize();
    }

    void VerticalShifter::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        subView.Paint(canvas, boundingRegion);
    }

    void VerticalShifter::ResetSize()
    {
        subView.ResetSize();
    }

    void VerticalShifter::ViewRegionChanged()
    {
        subView.SetViewRegion(infra::Region(ViewRegion().Top() + shift, ViewRegion().Left(), ViewRegion().Top() + shift + subView.ViewRegion().Height(), ViewRegion().Right()));
    }
}
