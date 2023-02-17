#include "preview/views/ViewFramedTextButton.hpp"

namespace services
{
    FramedTextButtonAttributes::FramedTextButtonAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour activatedBackgroundColour, infra::Colour textColour, infra::Font font)
        : frameColour(frameColour)
        , backgroundColour(backgroundColour)
        , activatedBackgroundColour(activatedBackgroundColour)
        , textColour(textColour)
        , font(font)
    {}

    ViewFramedTextButton::ViewFramedTextButton(FramedTextButtonAttributes attributes, infra::BoundedConstString text)
        : ViewFramedText({ attributes.frameColour, attributes.backgroundColour, attributes.textColour, attributes.font }, text)
        , backgroundColour(attributes.backgroundColour)
        , activatedBackgroundColour(attributes.activatedBackgroundColour)
    {}

    void ViewFramedTextButton::Activate()
    {
        SetBackgroundColour(activatedBackgroundColour);
    }

    void ViewFramedTextButton::Deactivate()
    {
        SetBackgroundColour(backgroundColour);
    }

    FramedTextButtonAttributes ViewFramedTextButton::Attributes() const
    {
        FramedTextAttributes textAttributes = ViewFramedText::Attributes();

        return FramedTextButtonAttributes{ textAttributes.frameColour, textAttributes.backgroundColour, activatedBackgroundColour, textAttributes.textColour, textAttributes.font };
    }
}
