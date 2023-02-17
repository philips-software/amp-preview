#include "preview/views/ViewAligned.hpp"

namespace services
{
    ViewAligned::ViewAligned(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
        : horizontalAlignment(horizontalAlignment)
        , verticalAlignment(verticalAlignment)
    {}

    infra::Vector ViewAligned::MinimumSize() const
    {
        return subView->MinimumSize();
    }

    infra::Vector ViewAligned::MaximumSize() const
    {
        return infra::Vector::Maximum();
    }

    void ViewAligned::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        subView->Paint(canvas, boundingRegion);
    }

    void ViewAligned::ResetSize()
    {
        subView->ResetSize();
        ResizeWithoutTrigger(subView->ViewRegion().Size());
    }

    void ViewAligned::SetSubView(View& newSubView)
    {
        subView = &newSubView;
        subView->SetParent(*this);
        Dirty(subView->ViewRegion());
    }

    void ViewAligned::SetHorizontalAlignment(HorizontalAlignment newHorizontalAlignment)
    {
        horizontalAlignment = newHorizontalAlignment;
    }

    void ViewAligned::SetVerticalAlignment(VerticalAlignment newVerticalAlignment)
    {
        verticalAlignment = newVerticalAlignment;
    }

    void ViewAligned::SetAlignment(HorizontalAlignment newHorizontalAlignment, VerticalAlignment newVerticalAlignment)
    {
        horizontalAlignment = newHorizontalAlignment;
        verticalAlignment = newVerticalAlignment;
    }

    void ViewAligned::ViewRegionChanged()
    {
        subView->SetViewRegion(infra::Region(infra::Point(ComputeOriginX(), ComputeOriginY()), subView->ViewRegion().Size()));
    }

    int16_t ViewAligned::ComputeOriginX() const
    {
        switch (horizontalAlignment)
        {
            case HorizontalAlignment::left:
                return 0;
            case HorizontalAlignment::right:
                return ViewRegion().Right() - subView->ViewRegion().Width();
            case HorizontalAlignment::centre:
                return ViewRegion().Left() + (ViewRegion().Width() - subView->ViewRegion().Width()) / 2;
            default:
                std::abort();
        }
    }

    int16_t ViewAligned::ComputeOriginY() const
    {
        switch (verticalAlignment)
        {
            case VerticalAlignment::top:
                return 0;
            case VerticalAlignment::centre:
                return ViewRegion().Top() + (ViewRegion().Height() - subView->ViewRegion().Height()) / 2;
            case VerticalAlignment::bottom:
                return ViewRegion().Bottom() - subView->ViewRegion().Height();
            default:
                std::abort();
        }
    }
}
