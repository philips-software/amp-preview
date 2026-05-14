#ifndef PREVIEW_VIEW_ICON_HPP
#define PREVIEW_VIEW_ICON_HPP

#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewIcon
        : public View
    {
    public:
        ViewIcon(const infra::SimpleBitmap& bitmap, infra::Colour colour);

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void Source(const infra::SimpleBitmap& newSource);
        const infra::SimpleBitmap& Source() const;
        void SetColour(infra::Colour newColour);

    private:
        const infra::SimpleBitmap* source;
        infra::Colour colour;
    };
}

#endif
