#ifndef PREVIEW_VIEW_FADING_TEXT_HPP
#define PREVIEW_VIEW_FADING_TEXT_HPP

#include "infra/timer/Timer.hpp"
#include "preview/views/ViewText.hpp"

namespace services
{
    struct FadingTextAttributes
        : TextAttributes
    {
        FadingTextAttributes(infra::Colour colour, infra::Colour background, infra::Font font = infra::freeSans12pt7b);

        infra::Colour background;
    };

    class ViewFadingText
        : public ViewText
    {
    public:
        template<std::size_t Size>
        using WithStorage = infra::WithStorage<infra::WithStorage<ViewFadingText, infra::BoundedString::WithStorage<Size>>, infra::BoundedString::WithStorage<Size>>;

        ViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, FadingTextAttributes attributes);
        ViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, FadingTextAttributes attributes, infra::BoundedConstString text);

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void SetText(infra::BoundedConstString newText);
        void SetTextAndResize(infra::BoundedConstString newText);

        void FadeIn(infra::BoundedConstString newText, Direction from);

        infra::Region TextRegion() const;
        std::optional<infra::Region> FadingRegion() const;

    private:
        infra::BoundedString& buffer1;
        infra::BoundedString& buffer2;
        infra::Colour background;

        infra::TimePoint fadeStart;
        Direction fadeDirection;
        infra::TimerRepeating fadeTimer;
    };
}

#endif
