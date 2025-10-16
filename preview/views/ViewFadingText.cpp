#include "preview/views/ViewFadingText.hpp"

namespace services
{
    FadingTextAttributes::FadingTextAttributes(infra::Colour colour, infra::Colour background, infra::Font font)
        : TextAttributes(colour, font)
        , background(background)
    {}

    ViewFadingText::ViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, FadingTextAttributes attributes)
        : ViewText(attributes)
        , buffer1(buffer1)
        , buffer2(buffer2)
        , background(attributes.background)
    {}

    ViewFadingText::ViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, FadingTextAttributes attributes, infra::BoundedConstString text)
        : ViewText(attributes)
        , buffer1(buffer1)
        , buffer2(buffer2)
        , background(attributes.background)
    {
        buffer2.assign(text);
        SetTextAndResize(buffer2);
    }

    void ViewFadingText::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        if (fadeTimer.Armed())
        {
            auto proportion = static_cast<uint16_t>(std::min<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(infra::Now() - fadeStart).count() * 256 / 250, 256));
            if (proportion < 256)
            {
                auto textOrigin = TextRegion().TopLeft() + infra::DeltaY(Font().cursorToTop);
                auto fadingOrigin = FadingRegion()->TopLeft() + infra::DeltaY(Font().cursorToTop);

                canvas.DrawString(fadingOrigin, buffer1, Font(), infra::BetweenColour(Attributes().colour, background, proportion), infra::RightAngle::angle_0, boundingRegion & ViewRegion());
                canvas.DrawString(textOrigin, buffer2, Font(), infra::BetweenColour(background, Attributes().colour, proportion), infra::RightAngle::angle_0, boundingRegion & ViewRegion());
            }
            else
                fadeTimer.Cancel();
        }

        if (!fadeTimer.Armed())
            ViewText::Paint(canvas, boundingRegion);
    }

    void ViewFadingText::SetText(infra::BoundedConstString newText)
    {
        buffer2.assign(newText);
        ViewText::SetText(newText);
    }

    void ViewFadingText::SetTextAndResize(infra::BoundedConstString newText)
    {
        buffer2.assign(newText);
        ViewText::SetTextAndResize(newText);
    }

    void ViewFadingText::FadeIn(infra::BoundedConstString newText, Direction from)
    {
        buffer1.assign(buffer2);
        buffer2.assign(newText.substr(0, buffer2.max_size()));
        SetText(buffer2);

        fadeStart = infra::Now();
        fadeDirection = from;
        fadeTimer.Start(
            std::chrono::milliseconds(40), [this]()
            {
                Dirty(ViewRegion());
            },
            infra::triggerImmediately);
    }

    infra::Region ViewFadingText::TextRegion() const
    {
        if (fadeTimer.Armed())
        {
            auto proportion = static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::milliseconds>(infra::Now() - fadeStart).count() * 256 / 250);
            if (proportion < 256)
            {
                int16_t offset = Font().newlineDistance * proportion / 256;
                infra::Vector deltaText{ static_cast<int16_t>(IsHorizontal(fadeDirection) ? offset - Font().newlineDistance : 0), static_cast<int16_t>(IsVertical(fadeDirection) ? offset - Font().newlineDistance : 0) };

                if (fadeDirection == Direction::down || fadeDirection == Direction::right)
                    deltaText = -deltaText;

                return ViewText::TextRegion() >> deltaText;
            }
        }

        return ViewText::TextRegion();
    }

    std::optional<infra::Region> ViewFadingText::FadingRegion() const
    {
        if (fadeTimer.Armed())
        {
            auto proportion = static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::milliseconds>(infra::Now() - fadeStart).count() * 256 / 250);
            if (proportion < 256)
            {
                int16_t offset = Font().newlineDistance * proportion / 256;

                infra::Vector deltaBuffer{ static_cast<int16_t>(IsHorizontal(fadeDirection) ? offset : 0), static_cast<int16_t>(IsVertical(fadeDirection) ? offset : 0) };

                if (fadeDirection == Direction::down || fadeDirection == Direction::right)
                    deltaBuffer = -deltaBuffer;

                return std::make_optional(ViewText::TextRegion() >> deltaBuffer);
            }
        }

        return std::nullopt;
    }
}
