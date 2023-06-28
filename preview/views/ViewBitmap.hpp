#ifndef PREVIEW_VIEW_BITMAP_HPP
#define PREVIEW_VIEW_BITMAP_HPP

#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewBitmap
        : public View
    {
    public:
        ViewBitmap(infra::Bitmap& bitmap);

        // Implementation of View
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

        void Source(infra::Bitmap& source);
        infra::Bitmap& Source() const;

    private:
        infra::Bitmap& source;
    };
}
#endif
