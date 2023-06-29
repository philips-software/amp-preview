#include "preview/views/ViewMultiple.hpp"

namespace services
{
    ViewMultiple::ViewMultiple(infra::BoundedVector<View*>& views)
        : views(views)
    {}

    infra::Vector ViewMultiple::MinimumSize() const
    {
        infra::Vector result;

        for (auto view : views)
        {
            auto minimumSize = view->MinimumSize();
            result.deltaX = std::max(result.deltaX, minimumSize.deltaX);
            result.deltaY = std::max(result.deltaY, minimumSize.deltaY);
        }

        return result;
    }

    infra::Vector ViewMultiple::MaximumSize() const
    {
        infra::Vector result(infra::Vector::Maximum());

        for (auto view : views)
        {
            auto maximumSize = view->MaximumSize();
            result.deltaX = std::min(result.deltaX, maximumSize.deltaX);
            result.deltaY = std::min(result.deltaY, maximumSize.deltaY);
        }

        return result;
    }

    void ViewMultiple::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        for (auto view : views)
            view->Paint(canvas, boundingRegion);
    }

    infra::Region ViewMultiple::DrawRegion() const
    {
        infra::Region result;

        for (auto view : views)
            result = infra::Union(result, view->DrawRegion());

        return result;
    }

    void ViewMultiple::ResetSize()
    {
        for (auto view : views)
            view->ResetSize();

        ResizeWithoutTrigger(MinimumSize());
    }

    void ViewMultiple::Add(View& view)
    {
        views.push_back(&view);
        view.SetParent(*this);
        view.ResetLayout(ViewRegion().Size());
        Dirty(view.DrawRegion());
    }

    void ViewMultiple::Remove(View& view)
    {
        Dirty(view.DrawRegion());
        views.erase(std::find(views.begin(), views.end(), &view));
    }

    void ViewMultiple::BringToFront(View& view)
    {
        auto iterator = std::find(views.begin(), views.end(), &view);
        assert(iterator != views.end());

        if (iterator != &views.back())
        {
            views.erase(iterator);
            views.push_back(&view);
            Dirty(view.DrawRegion());
        }
    }

    void ViewMultiple::ViewRegionChanged()
    {
        for (auto view : views)
            view->Resize(ViewRegion().Size());
    }
}
