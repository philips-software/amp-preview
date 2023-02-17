#ifndef PREVIEW_HORIZONTAL_LAYOUT_HPP
#define PREVIEW_HORIZONTAL_LAYOUT_HPP

#include "infra/util/BoundedVector.hpp"
#include "infra/util/WithStorage.hpp"
#include "preview/interfaces/View.hpp"
#include "preview/views/ViewAligned.hpp"

namespace services
{
    namespace detail
    {
        struct LayoutViewInfo;
    }

    class HorizontalLayout
        : public View
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<HorizontalLayout, infra::BoundedVector<detail::LayoutViewInfo>::WithMaxSize<N>>;

        HorizontalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, uint8_t interMargin = 0, uint8_t topMargin = 0, uint8_t rightMargin = 0, uint8_t bottomMargin = 0, uint8_t leftMargin = 0);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual infra::Region DrawRegion() const override;
        virtual void ResetSize() override;

        void Add(View& view, uint16_t proportion = 0, VerticalAlignment alignment = VerticalAlignment::top);
        void AddFill(View& view, uint16_t proportion = 0);
        void AddDummy(uint16_t proportion);

        void BringToFront(View& view);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        infra::BoundedVector<detail::LayoutViewInfo>& views;
        uint8_t interMargin;
        uint8_t topMargin;
        uint8_t rightMargin;
        uint8_t bottomMargin;
        uint8_t leftMargin;
    };

    namespace detail
    {
        struct LayoutViewInfo
        {
            View* view;
            infra::Optional<VerticalAlignment> alignment; // No alignment means fill up the available space in the vertical direction
            uint16_t proportion;
            uint8_t paintOrder;
            infra::Region drawRegion;

            bool maximize; // Used during layout
            bool minimize; // Used during layout
        };

        class HorizontalLayouter
        {
        public:
            HorizontalLayouter(infra::BoundedVector<LayoutViewInfo>& views, uint8_t interMargin);

            void Layout(infra::Region region, infra::Region& dirty);

        protected:
            virtual infra::Point AdjustOrientation(infra::Point point) const;
            virtual infra::Vector AdjustOrientation(infra::Vector vector) const;
            virtual infra::Region AdjustOrientation(infra::Region region) const;

        private:
            uint16_t TotalProportion() const;
            void ClearViewInfos();
            uint16_t StaticNeededX() const;
            void EliminateLimitedProportionalViews(infra::Region region);
            uint16_t ComputeTooSmall() const;
            uint16_t ComputeTooBig() const;
            void FixateMaximumSizeViews();
            void FixateMinimumSizeViews();
            void AllocateViews(infra::Region region, infra::Region& dirty);
            std::pair<uint16_t, int16_t> ComputeVerticalPosition(infra::Region region, LayoutViewInfo& viewInfo, infra::Point startOrigin) const;
            uint16_t ComputeWidth(const LayoutViewInfo& viewInfo, uint16_t& availableResizableX, uint16_t& runningProportion) const;

        private:
            infra::BoundedVector<LayoutViewInfo>& views;
            uint8_t interMargin;
            uint16_t totalProportion;
            uint16_t availableResizableX;
        };
    }
}

#endif
