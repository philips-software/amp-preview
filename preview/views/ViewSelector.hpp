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

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

    protected:
        virtual void ViewRegionChanged() override;

    private:
        infra::BoundedVector<View*>& views;
        infra::BoundedVector<View*>::iterator currentView;
    };
}
#endif
