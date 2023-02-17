#ifndef PREVIEW_VIEW_FRAMED_TEXT_BUTTON_HPP
#define PREVIEW_VIEW_FRAMED_TEXT_BUTTON_HPP

#include "preview/interfaces/Button.hpp"
#include "preview/views/ViewFramedText.hpp"

namespace services
{
    struct FramedTextButtonAttributes
    {
        FramedTextButtonAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour activatedBackgroundColour, infra::Colour textColour = infra::Colour::white, infra::Font font = infra::freeSans12pt7b);

        infra::Colour frameColour;
        infra::Colour backgroundColour;
        infra::Colour activatedBackgroundColour;
        infra::Colour textColour;
        infra::Font font;
    };

    class ViewFramedTextButton
        : public Button
        , public ViewFramedText
    {
    public:
        ViewFramedTextButton(FramedTextButtonAttributes attributes, infra::BoundedConstString text);

        // Implementation of Button
        virtual void Activate() override;
        virtual void Deactivate() override;

        FramedTextButtonAttributes Attributes() const;

    private:
        infra::Colour backgroundColour;
        infra::Colour activatedBackgroundColour;
    };
}

#endif
