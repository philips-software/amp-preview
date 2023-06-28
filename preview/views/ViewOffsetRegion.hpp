#ifndef PREVIEW_VIEW_OFFSET_REGION_HPP
#define PREVIEW_VIEW_OFFSET_REGION_HPP

#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewOffsetRegion
        : public View
    {
    public:
        template<class T>
        class WithView;

        explicit ViewOffsetRegion(infra::RegionOffset regionOffset);

        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        infra::Region DrawRegion() const override;
        void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetOffset(infra::RegionOffset newRegionOffset);

    protected:
        void ViewRegionChanged() override;

    private:
        View* subView = nullptr;
        infra::RegionOffset regionOffset;
    };

    template<class T>
    class ViewOffsetRegion::WithView
        : public ViewOffsetRegion
    {
    public:
        template<class... Args>
        WithView(infra::RegionOffset regionOffset, Args&&... args)
            : ViewOffsetRegion(regionOffset)
            , subView(std::forward<Args>(args)...)
        {
            SetSubView(subView);
        }

        T& SubView()
        {
            return subView;
        }

    private:
        T subView;
    };
}

#endif
