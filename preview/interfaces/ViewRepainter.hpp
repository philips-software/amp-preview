#ifndef PREVIEW_VIEW_REPAINTER_HPP
#define PREVIEW_VIEW_REPAINTER_HPP

#include "preview/interfaces/ViewPainter.hpp"

namespace services
{
    class ViewRepainterPaintWhenDirty
        : public View
    {
    public:
        ViewRepainterPaintWhenDirty(ViewPainter& painter, View& topView);

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;

    protected:
        void Dirty(infra::Region region) override;

    private:
        void RepaintDone();

    private:
        ViewPainter& painter;
        View& topView;

        bool busy = false;
        infra::Region dirty;
    };
}

#endif
