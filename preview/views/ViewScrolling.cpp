#include "preview/views/ViewScrolling.hpp"

namespace services
{
    ViewScrolling::ViewScrolling(View& subView, infra::Duration pixelSpeed, uint32_t pixelsPerStep, infra::Duration hold)
        : subView(subView)
        , pixelSpeed(pixelSpeed)
        , hold(hold)
        , pixelsPerStep(pixelsPerStep)
    {
        subView.SetParent(*this);
        ResizeWithoutTrigger(infra::Vector(0, subView.ViewRegion().Size().deltaY));
        FirstStep();
    }

    infra::Vector ViewScrolling::MinimumSize() const
    {
        return infra::Vector(0, subView.MinimumSize().deltaY);
    }

    infra::Vector ViewScrolling::MaximumSize() const
    {
        return infra::Vector(infra::Vector::Maximum().deltaX, subView.MaximumSize().deltaY);
    }

    void ViewScrolling::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        subView.Paint(canvas, boundingRegion & ViewRegion());
    }

    void ViewScrolling::ResetSize()
    {
        subView.ResetSize();
        ResizeWithoutTrigger(infra::Vector(0, subView.ViewRegion().Size().deltaY));
    }

    void ViewScrolling::ViewRegionChanged()
    {
        SetOffset(offset);
    }

    void ViewScrolling::FirstStep()
    {
        SetOffset(0);
        timer.Start(hold, [this]()
            { timer.Start(pixelSpeed / pixelsPerStep, [this]()
                  { Step(); }); });
    }

    void ViewScrolling::Step()
    {
        auto maxScroll = subView.ViewRegion().Width() - ViewRegion().Width();
        if (offset >= maxScroll)
            timer.Start(hold, [this]()
                { FirstStep(); });
        else
            SetOffset(std::min(offset + pixelsPerStep, maxScroll));
    }

    void ViewScrolling::SetOffset(uint16_t newOffset)
    {
        offset = newOffset;

        auto region = ViewRegion();
        region.Resize(infra::DeltaX(subView.ViewRegion().Size().deltaX));
        region >>= infra::DeltaX(-offset);

        subView.SetViewRegion(region);
    }
}
