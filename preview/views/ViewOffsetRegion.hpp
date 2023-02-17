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

        ViewOffsetRegion(infra::RegionOffset regionOffset);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual infra::Region DrawRegion() const override;
        virtual void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetOffset(infra::RegionOffset newRegionOffset);

    protected:
        virtual void ViewRegionChanged() override;

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
