#include "preview/views/ViewFrame.hpp"

namespace services
{
    ViewFrame::ViewFrame(FrameAttributes attributes)
        : frameColour(attributes.frameColour)
        , panel(attributes.backgroundColour)
    {
        panel.SetParent(*this);
    }

    infra::Vector ViewFrame::MinimumSize() const
    {
        return panel.MinimumSize() + infra::Vector(2, 2);
    }

    infra::Vector ViewFrame::MaximumSize() const
    {
        return infra::RestrictedSum(infra::Vector(2, 2), panel.MaximumSize());
    }

    void ViewFrame::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        panel.Paint(canvas, boundingRegion);
        canvas.DrawRectangle(ViewRegion(), frameColour, boundingRegion);
    }

    void ViewFrame::ResetSize()
    {
        panel.ResetSize();
        ResizeWithoutTrigger(infra::RestrictedSum(infra::Vector(2, 2), panel.ViewRegion().Size()));
    }

    void ViewFrame::SetSubView(View& newSubView)
    {
        panel.SetSubView(newSubView);
    }

    void ViewFrame::SetFrameColour(infra::Colour newColour)
    {
        frameColour = newColour;
        Dirty(ViewRegion());
    }

    void ViewFrame::SetBackgroundColour(infra::Colour newColour)
    {
        panel.SetColour(newColour);
    }

    void ViewFrame::ViewRegionChanged()
    {
        panel.SetViewRegion((ViewRegion() >> infra::Vector(1, 1)) - infra::Vector(2, 2));
    }
}
