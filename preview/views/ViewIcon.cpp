#include "preview/views/ViewIcon.hpp"

namespace services
{
    ViewIcon::ViewIcon(const infra::Bitmap& bitmap, infra::Colour colour)
        : source(&bitmap)
        , colour(colour)
    {
        Resize(bitmap.size);
    }

    void ViewIcon::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawIcon(ViewRegion().TopLeft(), *source, colour, ViewRegion() & boundingRegion);
    }

    void ViewIcon::Source(const infra::Bitmap& newSource)
    {
        source = &newSource;
        Dirty(ViewRegion());
    }

    const infra::Bitmap& ViewIcon::Source() const
    {
        return *source;
    }

    void ViewIcon::SetColour(infra::Colour newColour)
    {
        colour = newColour;
        Dirty(ViewRegion());
    }
}
