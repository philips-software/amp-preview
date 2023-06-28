#ifndef VIEWSELECTOR_HPP
#define VIEWSELECTOR_HPP

#include "infra/util/BoundedVector.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewSelector
        : public View
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<ViewSelector, infra::BoundedVector<View*>::WithMaxSize<N>>;

        ViewSelector(infra::BoundedVector<View*>& views);

        void Add(View& screen);
        void Remove(View& screen);
        void Next();
        void Previous();
        View& Current() const;

        // Implementation of View
        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        void ResetSize() override;

    protected:
        void ViewRegionChanged() override;

    private:
        infra::BoundedVector<View*>& views;
        infra::BoundedVector<View*>::iterator currentView;
    };
}

#endif
