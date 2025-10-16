#include "preview/views/ViewFramedTextButtonWithPopOut.hpp"

namespace services
{
    ViewFramedTextButtonWithPopOut::ViewFramedTextButtonWithPopOut(FramedTextButtonAttributes attributes, infra::BoundedConstString text, infra::RegionOffset regionOffset)
        : ViewFramedTextButton(attributes, text)
        , regionOffset(regionOffset)
    {}

    void ViewFramedTextButtonWithPopOut::Activate()
    {
        ViewFramedTextButton::Activate();

        FramedTextButtonAttributes attributes = Attributes();
        popOut.emplace(regionOffset, FramedTextAttributes{ attributes.frameColour, attributes.activatedBackgroundColour, attributes.textColour, attributes.font }, Text());
        popOut->SetParent(*this);
        popOut->SetViewRegion(ViewRegion());
        Dirty(popOut->ViewRegion());
    }

    void ViewFramedTextButtonWithPopOut::Deactivate()
    {
        Dirty(popOut->DrawRegion());
        popOut.reset();

        ViewFramedTextButton::Deactivate();
    }

    void ViewFramedTextButtonWithPopOut::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        ViewFramedTextButton::Paint(canvas, boundingRegion);

        if (popOut)
            popOut->Paint(canvas, boundingRegion);
    }

    void ViewFramedTextButtonWithPopOut::ResetSize()
    {
        ViewFramedTextButton::ResetSize();

        if (popOut)
            popOut->ResetSize();
    }

    void ViewFramedTextButtonWithPopOut::ViewRegionChanged()
    {
        ViewFramedTextButton::ViewRegionChanged();

        if (popOut != std::nullopt)
            popOut->SetViewRegion(ViewRegion());
    }
}
