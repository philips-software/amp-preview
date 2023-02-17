#include "preview/views/ViewFramedText.hpp"

namespace services
{
    FramedTextAttributes::FramedTextAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour textColour, infra::Font font)
        : frameColour(frameColour)
        , backgroundColour(backgroundColour)
        , textColour(textColour)
        , font(font)
    {}

    ViewFramedText::ViewFramedText(FramedTextAttributes attributes, infra::BoundedConstString text)
        : ViewText(TextAttributes{ attributes.textColour, attributes.font }, text)
        , frameColour(attributes.frameColour)
        , backgroundColour(attributes.backgroundColour)
    {
        Resize(ViewRegion().Size() + infra::Vector(2, 2));
    }

    infra::Vector ViewFramedText::MinimumSize() const
    {
        return infra::Vector(2, 2);
    }

    void ViewFramedText::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        infra::Region viewRegion = ViewRegion();
        canvas.DrawFilledRectangle(viewRegion, backgroundColour, boundingRegion);
        canvas.DrawRectangle(viewRegion, frameColour, boundingRegion);

        viewRegion = (viewRegion >> infra::Vector(1, 1)) - infra::Vector(2, 2);
        ViewText::Paint(canvas, boundingRegion & viewRegion);
    }

    void ViewFramedText::SetFrameColour(infra::Colour newFrameColour)
    {
        frameColour = newFrameColour;
        Dirty(ViewRegion());
    }

    void ViewFramedText::SetBackgroundColour(infra::Colour newBackgroundColour)
    {
        backgroundColour = newBackgroundColour;
        Dirty(ViewRegion());
    }

    FramedTextAttributes ViewFramedText::Attributes() const
    {
        TextAttributes textAttributes = ViewText::Attributes();

        return FramedTextAttributes{ frameColour, backgroundColour, textAttributes.colour, textAttributes.font };
    }
}
