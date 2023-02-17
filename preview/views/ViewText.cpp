#include "preview/views/ViewText.hpp"

namespace services
{
    TextAttributes::TextAttributes(infra::Colour colour, infra::Font font)
        : colour(colour)
        , font(font)
    {}

    ViewText::ViewText(TextAttributes attributes)
        : font(attributes.font)
        , colour(attributes.colour)
    {
        Resize(infra::Vector(0, font.newlineDistance));
    }

    ViewText::ViewText(TextAttributes attributes, infra::BoundedConstString text)
        : font(attributes.font)
        , colour(attributes.colour)
    {
        Resize(infra::Vector(0, font.newlineDistance));
        SetTextAndResize(text);
    }

    void ViewText::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawString(textOrigin, text, font, colour, infra::RightAngle::angle_0, boundingRegion & ViewRegion());
    }

    void ViewText::SetHorizontalAlignment(HorizontalAlignment newHorizontalAlignment)
    {
        SetAlignment(newHorizontalAlignment, verticalAlignment);
    }

    void ViewText::SetVerticalAlignment(VerticalAlignment newVerticalAlignment)
    {
        SetAlignment(horizontalAlignment, newVerticalAlignment);
    }

    void ViewText::SetAlignment(HorizontalAlignment newHorizontalAlignment, VerticalAlignment newVerticalAlignment)
    {
        horizontalAlignment = newHorizontalAlignment;
        verticalAlignment = newVerticalAlignment;
        ComputeTextOrigin();
        Dirty(ViewRegion());
    }

    void ViewText::SetText(infra::BoundedConstString newText)
    {
        text = newText;
        ComputeTextOrigin();
        Dirty(ViewRegion());
    }

    void ViewText::SetTextAndResize(infra::BoundedConstString newText)
    {
        Resize(infra::Vector(font.Width(newText), ViewRegion().Size().deltaY));
        SetText(newText);
    }

    void ViewText::SetTextColour(infra::Colour newColour)
    {
        colour = newColour;
        Dirty(TextRegion());
    }

    infra::Point ViewText::TextOrigin() const
    {
        return textOrigin;
    }

    infra::Region ViewText::TextRegion() const
    {
        return infra::Region(textOrigin - infra::DeltaY(font.cursorToTop), infra::Vector(font.Width(text), font.newlineDistance));
    }

    TextAttributes ViewText::Attributes() const
    {
        return TextAttributes{ colour, font };
    }

    infra::BoundedConstString ViewText::Text() const
    {
        return text;
    }

    const infra::Font& ViewText::Font() const
    {
        return font;
    }

    void ViewText::ViewRegionChanged()
    {
        ComputeTextOrigin();
    }

    void ViewText::ComputeTextOrigin()
    {
        textOrigin = ViewRegion().TopLeft() + infra::Vector(ComputeOriginX(), ComputeOriginY());
    }

    int16_t ViewText::ComputeOriginX() const
    {
        switch (horizontalAlignment)
        {
            case HorizontalAlignment::left:
                return 0;
            case HorizontalAlignment::right:
                return ViewRegion().Size().deltaX - font.Width(text);
            case HorizontalAlignment::centre:
                return (ViewRegion().Size().deltaX - font.Width(text)) / 2;
            default:
                std::abort();
        }
    }

    int16_t ViewText::ComputeOriginY() const
    {
        switch (verticalAlignment)
        {
            case VerticalAlignment::top:
                return font.cursorToTop;
            case VerticalAlignment::centre:
                return (font.cursorToTop + ViewRegion().Height() - 1) / 2;
            case VerticalAlignment::bottom:
                return ViewRegion().Height() - 1;
            default:
                std::abort();
        }
    }
}
