#include "preview/views/ViewTitledFrame.hpp"

namespace services
{
    TitledFrameAttributes::TitledFrameAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour textColour, infra::Font font)
        : frameColour(frameColour)
        , backgroundColour(backgroundColour)
        , textColour(textColour)
        , font(font)
    {}

    ViewTitledFrame::ViewTitledFrame(TitledFrameAttributes attributes, infra::BoundedConstString text)
        : text(FramedTextAttributes{ attributes.frameColour, attributes.backgroundColour, attributes.textColour, attributes.font }, text)
        , frame(FrameAttributes{ attributes.frameColour, attributes.backgroundColour })
    {
        this->text.SetParent(*this);
        frame.SetParent(*this);
    }

    infra::Vector ViewTitledFrame::MinimumSize() const
    {
        infra::Vector textSize = text.MinimumSize();
        infra::Vector frameSize = frame.MinimumSize();

        return infra::Vector(std::max(textSize.deltaX, frameSize.deltaX), textSize.deltaY + frameSize.deltaY) - infra::DeltaY(1);
    }

    infra::Vector ViewTitledFrame::MaximumSize() const
    {
        infra::Vector textSize = text.MaximumSize();
        infra::Vector frameSize = frame.MaximumSize();

        return infra::RestrictedSum(infra::Vector(std::min(textSize.deltaX, frameSize.deltaX), infra::RestrictedInt16Sum(textSize.deltaY, frameSize.deltaY)), infra::Vector(2, 3));
    }

    void ViewTitledFrame::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        text.Paint(canvas, boundingRegion);
        frame.Paint(canvas, boundingRegion);
    }

    void ViewTitledFrame::ResetSize()
    {
        frame.ResetSize();

        infra::Vector textSize = text.ViewRegion().Size();
        infra::Vector frameSize = frame.ViewRegion().Size();

        ResizeWithoutTrigger(infra::Vector(std::max(textSize.deltaX, frameSize.deltaX), textSize.deltaY + frameSize.deltaY) - infra::DeltaY(1));
    }

    void ViewTitledFrame::SetSubView(View& newSubView)
    {
        frame.SetSubView(newSubView);
    }

    void ViewTitledFrame::SetFrameColour(infra::Colour newColour)
    {
        text.SetFrameColour(newColour);
        frame.SetFrameColour(newColour);
    }

    void ViewTitledFrame::SetBackgroundColour(infra::Colour newColour)
    {
        text.SetBackgroundColour(newColour);
        frame.SetBackgroundColour(newColour);
    }

    void ViewTitledFrame::SetTextColour(infra::Colour newColour)
    {
        text.SetTextColour(newColour);
    }

    void ViewTitledFrame::ViewRegionChanged()
    {
        infra::Region region(ViewRegion());
        text.SetViewRegion(infra::Region(region.Origin(), infra::Vector(region.Size().deltaX, text.Font().newlineDistance)));
        frame.SetViewRegion(infra::Region(region.Origin() + infra::DeltaY(text.Font().newlineDistance - 1), region.Size() - infra::DeltaY(text.Font().newlineDistance - 1)));
    }
}
