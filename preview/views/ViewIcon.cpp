#include "preview/views/ViewIcon.hpp"

namespace services
{
    ViewIcon::ViewIcon(const infra::SimpleBitmap& bitmap, infra::Colour colour)
        : source(&bitmap)
        , colour(colour)
    {
        Resize(bitmap.size);
    }

    void ViewIcon::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawIcon(ViewRegion().TopLeft(), *source, colour, ViewRegion() & boundingRegion);
    }

    void ViewIcon::Source(const infra::SimpleBitmap& newSource)
    {
        source = &newSource;
        Dirty(ViewRegion());
    }

    const infra::SimpleBitmap& ViewIcon::Source() const
    {
        return *source;
    }

    void ViewIcon::SetColour(infra::Colour newColour)
    {
        colour = newColour;
        Dirty(ViewRegion());
    }
}
