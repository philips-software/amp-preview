#ifndef PREVIEW_VIEW_FRAMED_TEXT_HPP
#define PREVIEW_VIEW_FRAMED_TEXT_HPP

#include "preview/views/ViewText.hpp"

namespace services
{
    struct FramedTextAttributes
    {
        FramedTextAttributes(infra::Colour frameColour, infra::Colour backgroundColour, infra::Colour textColour = infra::Colour::white, infra::Font font = infra::freeSans12pt7b);

        infra::Colour frameColour;
        infra::Colour backgroundColour;
        infra::Colour textColour;
        infra::Font font;
    };

    class ViewFramedText
        : public ViewText
    {
    public:
        ViewFramedText(FramedTextAttributes attributes, infra::BoundedConstString text);

        infra::Vector MinimumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void SetFrameColour(infra::Colour newFrameColour);
        void SetBackgroundColour(infra::Colour newBackgroundColour);

        FramedTextAttributes Attributes() const;

    private:
        infra::Colour frameColour;
        infra::Colour backgroundColour;
    };
}

#endif
