#include "preview/views/ViewDisableableText.hpp"

namespace services
{
    void ViewDisableableText::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        ViewText::Paint(canvas, boundingRegion);

        if (!enabled)
        {
            auto from = TextRegion().BottomLeft() - infra::DeltaY(Font().newlineDistance - Font().cursorToTop);
            auto to = TextRegion().TopRight();
            canvas.DrawLine(from, to, infra::Colour::black, boundingRegion);
            canvas.DrawLine(from - infra::DeltaY(1), to - infra::DeltaX(1), infra::Colour::black, boundingRegion & TextRegion());
            canvas.DrawLine(from + infra::DeltaX(1), to + infra::DeltaY(1), infra::Colour::black, boundingRegion & TextRegion());
        }
    }

    void ViewDisableableText::Enable(bool newEnabled)
    {
        Dirty(TextRegion());
        enabled = newEnabled;
    }

    void ViewDisableableFadingText::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        ViewFadingText::Paint(canvas, boundingRegion);

        if (!enabled)
        {
            DrawLine(TextRegion(), canvas, boundingRegion);

            auto fadingRegion = FadingRegion();
            if (fadingRegion != std::nullopt)
                DrawLine(*fadingRegion, canvas, boundingRegion);
        }
    }

    void ViewDisableableFadingText::Enable(bool newEnabled)
    {
        Dirty(TextRegion() | FadingRegion().value_or(infra::Region()));
        enabled = newEnabled;
    }

    void ViewDisableableFadingText::DrawLine(infra::Region region, hal::Canvas& canvas, infra::Region boundingRegion)
    {
        auto from = region.BottomLeft() - infra::DeltaY(Font().newlineDistance - Font().cursorToTop);
        auto to = region.TopRight();
        canvas.DrawLine(from, to, infra::Colour::black, boundingRegion);
        canvas.DrawLine(from - infra::DeltaY(1), to - infra::DeltaX(1), infra::Colour::black, boundingRegion);
        canvas.DrawLine(from + infra::DeltaX(1), to + infra::DeltaY(1), infra::Colour::black, boundingRegion);
    }
}
