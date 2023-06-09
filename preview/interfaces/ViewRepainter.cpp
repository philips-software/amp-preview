#include "preview/interfaces/ViewRepainter.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/PostAssign.hpp"

namespace services
{
    ViewRepainterPaintWhenDirty::ViewRepainterPaintWhenDirty(ViewPainter& painter, View& topView)
        : painter(painter)
        , topView(topView)
    {
        topView.SetParent(*this);
    }

    void ViewRepainterPaintWhenDirty::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        // All paints are initiated from this top-level view, so this Paint method will never be invoked
        std::abort();
    }

    void ViewRepainterPaintWhenDirty::Dirty(infra::Region region)
    {
        dirty = dirty | region;

        if (!busy)
        {
            busy = true;
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    painter.Paint(topView, infra::PostAssign(dirty, infra::Region()), [this]()
                        {
                            RepaintDone();
                        });
                });
        }
    }

    void ViewRepainterPaintWhenDirty::RepaintDone()
    {
        busy = false;
        if (!dirty.Empty())
            Dirty(dirty);
    }
}
