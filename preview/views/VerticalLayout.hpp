#ifndef PREVIEW_VERTICAL_LAYOUT_HPP
#define PREVIEW_VERTICAL_LAYOUT_HPP

#include "preview/views/HorizontalLayout.hpp"

namespace services
{
    class VerticalLayout
        : public View
    {
    public:
        template<std::size_t N>
        using WithMaxViews = infra::WithStorage<VerticalLayout, infra::BoundedVector<detail::LayoutViewInfo>::WithMaxSize<N>>;

        VerticalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, uint8_t interMargin = 0, uint8_t topMargin = 0, uint8_t rightMargin = 0, uint8_t bottomMargin = 0, uint8_t leftMargin = 0);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

        void Add(View& view, uint16_t proportion = 0, HorizontalAlignment alignment = HorizontalAlignment::left);
        void AddFill(View& view, uint16_t proportion = 0);
        void AddDummy(uint16_t proportion);

        void BringToFront(View& view);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        VerticalAlignment Flip(HorizontalAlignment alignment) const;

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
        class VerticalLayouter
            : public HorizontalLayouter
        {
        public:
            using HorizontalLayouter::HorizontalLayouter;

        protected:
            virtual infra::Point AdjustOrientation(infra::Point point) const;
            virtual infra::Vector AdjustOrientation(infra::Vector vector) const;
            virtual infra::Region AdjustOrientation(infra::Region region) const;
        };
    }
}

#endif
