#ifndef PREVIEW_VIEW_DISABLEABLE_TEXT_HPP
#define PREVIEW_VIEW_DISABLEABLE_TEXT_HPP

#include "preview/views/ViewFadingText.hpp"

namespace services
{
    class ViewDisableableText
        : public ViewText
    {
    public:
        using ViewText::ViewText;

        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void Enable(bool newEnabled);

    private:
        bool enabled = true;
    };

    class ViewDisableableFadingText
        : public ViewFadingText
    {
    public:
        using ViewFadingText::ViewFadingText;

        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void Enable(bool newEnabled);

    private:
        void DrawLine(infra::Region region, hal::Canvas& canvas, infra::Region boundingRegion);

    private:
        bool enabled = true;
    };
}

#endif
