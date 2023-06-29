#ifndef PREVIEW_VIEW_MULTIPLE_HPP
#define PREVIEW_VIEW_MULTIPLE_HPP

#include "infra/util/BoundedVector.hpp"
#include "infra/util/WithStorage.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewMultiple
        : public View
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<ViewMultiple, infra::BoundedVector<View*>::WithMaxSize<N>>;

        explicit ViewMultiple(infra::BoundedVector<View*>& views);

        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        infra::Region DrawRegion() const override;
        void ResetSize() override;

        void Add(View& view);
        void Remove(View& view);
        void BringToFront(View& view);

    protected:
        void ViewRegionChanged() override;

    private:
        infra::BoundedVector<View*>& views;
    };
}

#endif
