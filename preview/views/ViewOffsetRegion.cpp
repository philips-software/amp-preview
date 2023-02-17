#include "preview/views/ViewOffsetRegion.hpp"

namespace services
{
    ViewOffsetRegion::ViewOffsetRegion(infra::RegionOffset regionOffset)
        : regionOffset(regionOffset)
    {}

    infra::Vector ViewOffsetRegion::MinimumSize() const
    {
        if (subView != nullptr)
            return subView->MinimumSize() / regionOffset;
        else
            return infra::Vector();
    }

    infra::Vector ViewOffsetRegion::MaximumSize() const
    {
        if (subView != nullptr)
            return subView->MaximumSize() / regionOffset;
        else
            return infra::Vector();
    }

    void ViewOffsetRegion::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        if (subView != nullptr)
            subView->Paint(canvas, boundingRegion);
    }

    infra::Region ViewOffsetRegion::DrawRegion() const
    {
        if (subView != nullptr)
            return subView->DrawRegion();
        else
            return infra::Region();
    }

    void ViewOffsetRegion::ResetSize()
    {
        if (subView != nullptr)
        {
            subView->ResetSize();
            infra::Vector newSize = subView->ViewRegion().Size() / regionOffset;
            ResizeWithoutTrigger(infra::Vector(std::max<int16_t>(newSize.deltaX, 0), std::max<int16_t>(newSize.deltaY, 0)));
        }
        else
            ResizeWithoutTrigger(infra::Vector());
    }

    void ViewOffsetRegion::SetSubView(View& newSubView)
    {
        subView = &newSubView;
        subView->SetParent(*this);
        Dirty(subView->ViewRegion());
    }

    void ViewOffsetRegion::SetOffset(infra::RegionOffset newRegionOffset)
    {
        regionOffset = newRegionOffset;
    }

    void ViewOffsetRegion::ViewRegionChanged()
    {
        if (subView != nullptr)
            subView->SetViewRegion(ViewRegion() + regionOffset);
    }
}
