#include "ViewSelector.hpp"

namespace services
{
    ViewSelector::ViewSelector(infra::BoundedVector<View*>& views)
        : views(views)
        , currentView(views.begin())
    {}

    void ViewSelector::Add(View& view)
    {
        views.push_back(&view);
        view.SetParent(*this);
    }

    void ViewSelector::Remove(View& view)
    {
        auto index = std::find(views.begin(), views.end(), &view);
        assert(index != views.end());

        if (currentView >= index && currentView != views.begin())
            --currentView;
        views.erase(index);

        Dirty(ViewRegion());
    }

    void ViewSelector::Next()
    {
        ++currentView;
        if (currentView == views.end())
            currentView = views.begin();

        Dirty(ViewRegion());
    }

    void ViewSelector::Previous()
    {
        if (currentView == views.begin())
            currentView = views.end();
        --currentView;

        Dirty(ViewRegion());
    }

    View& ViewSelector::Current() const
    {
        return **currentView;
    }

    infra::Vector ViewSelector::MinimumSize() const
    {
        if (views.empty())
            return infra::Vector();

        return (*currentView)->MinimumSize();
    }

    infra::Vector ViewSelector::MaximumSize() const
    {
        if (views.empty())
            return infra::Vector::Maximum();

        return (*currentView)->MaximumSize();
    }

    void ViewSelector::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        if (!views.empty())
            (*currentView)->Paint(canvas, boundingRegion);
    }

    void ViewSelector::ResetSize()
    {
        if (!views.empty())
        {
            (*currentView)->ResetSize();
            ResizeWithoutTrigger((*currentView)->ViewRegion().Size());
        }
        else
            ResizeWithoutTrigger(infra::Vector());
    }

    void ViewSelector::ViewRegionChanged()
    {
        for (auto view : views)
            view->SetViewRegion(ViewRegion());
    }
}
