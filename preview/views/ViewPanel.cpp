#include "preview/views/ViewPanel.hpp"

namespace services
{
    ViewPanel::ViewPanel(infra::Colour colour)
        : colour(colour)
    {}

    infra::Vector ViewPanel::MinimumSize() const
    {
        if (subView)
            return subView->MinimumSize();
        else
            return infra::Vector();
    }

    infra::Vector ViewPanel::MaximumSize() const
    {
        if (subView)
            return subView->MaximumSize();
        else
            return infra::Vector::Maximum();
    }

    void ViewPanel::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawFilledRectangle(ViewRegion(), colour, boundingRegion);
        if (subView != nullptr)
            subView->Paint(canvas, boundingRegion);
    }

    void ViewPanel::ResetSize()
    {
        if (subView)
        {
            subView->ResetSize();
            ResizeWithoutTrigger(subView->ViewRegion().Size());
        }
        else
            ResizeWithoutTrigger(infra::Vector());
    }

    void ViewPanel::SetSubView(View& newSubView)
    {
        subView = &newSubView;
        subView->SetParent(*this);
        Dirty(subView->ViewRegion());
    }

    void ViewPanel::SetColour(infra::Colour newColour)
    {
        colour = newColour;
        Dirty(ViewRegion());
    }

    void ViewPanel::ViewRegionChanged()
    {
        if (subView != nullptr)
            subView->SetViewRegion(ViewRegion());
    }
}
