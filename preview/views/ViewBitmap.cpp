#include "preview/views/ViewBitmap.hpp"

namespace services
{
    ViewBitmap::ViewBitmap(infra::Bitmap& bitmap)
        : source(bitmap)
    {
        Resize(bitmap.size);
    }

    void ViewBitmap::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawBitmap(ViewRegion().TopLeft(), source, ViewRegion() & boundingRegion);
    }

    void ViewBitmap::Source(infra::Bitmap& source)
    {
        this->source = source;
        Dirty(ViewRegion());
    }

    infra::Bitmap& ViewBitmap::Source() const
    {
        return source;
    }
}
