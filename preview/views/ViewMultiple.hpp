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

        ViewMultiple(infra::BoundedVector<View*>& views);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual infra::Region DrawRegion() const override;
        virtual void ResetSize() override;

        void Add(View& view);
        void Remove(View& view);
        void BringToFront(View& view);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        infra::BoundedVector<View*>& views;
    };
}

#endif
