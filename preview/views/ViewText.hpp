#ifndef PREVIEW_VIEW_TEXT_HPP
#define PREVIEW_VIEW_TEXT_HPP

#include "preview/fonts/Fonts.hpp"
#include "preview/views/ViewAligned.hpp"

namespace services
{
    struct TextAttributes
    {
        TextAttributes(infra::Colour colour, infra::Font font = infra::freeSans12pt7b);

        infra::Colour colour;
        infra::Font font;
    };

    class ViewText
        : public View
    {
    public:
        ViewText(TextAttributes attributes);
        ViewText(TextAttributes attributes, infra::BoundedConstString text);

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void SetHorizontalAlignment(HorizontalAlignment newHorizontalAlignment);
        void SetVerticalAlignment(VerticalAlignment newVerticalAlignment);
        void SetAlignment(HorizontalAlignment newHorizontalAlignment, VerticalAlignment newVerticalAlignment);

        void SetText(infra::BoundedConstString newText);
        void SetTextAndResize(infra::BoundedConstString newText);
        void SetTextColour(infra::Colour newColour);

        infra::Point TextOrigin() const;
        infra::Region TextRegion() const;
        TextAttributes Attributes() const;
        infra::BoundedConstString Text() const;
        const infra::Font& Font() const;

    protected:
        void ViewRegionChanged() override;

    private:
        void ComputeTextOrigin();
        int16_t ComputeOriginX() const;
        int16_t ComputeOriginY() const;

    private:
        infra::BoundedConstString text;
        infra::Font font;
        infra::Colour colour;
        infra::Point textOrigin;
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::centre;
        VerticalAlignment verticalAlignment = VerticalAlignment::centre;
    };
}

#endif
