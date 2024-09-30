#include "preview/views/ViewBitmap.hpp"

namespace services
{
    ViewBitmap::ViewBitmap(const infra::Bitmap& bitmap)
        : source(bitmap)
    {
        Resize(bitmap.size);
    }

    void ViewBitmap::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        canvas.DrawBitmap(ViewRegion().TopLeft(), source, ViewRegion() & boundingRegion);
    }

    void ViewBitmap::Source(const infra::Bitmap& source)
    {
        this->source = source;
        Dirty(ViewRegion());
    }

    const infra::Bitmap& ViewBitmap::Source() const
    {
        return source;
    }
}
