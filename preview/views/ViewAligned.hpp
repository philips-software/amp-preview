#ifndef PREVIEW_VIEW_ALIGNED_HPP
#define PREVIEW_VIEW_ALIGNED_HPP

#include "preview/interfaces/Colour.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    enum class HorizontalAlignment : uint8_t
    {
        left,
        centre,
        right
    };

    enum class VerticalAlignment : uint8_t
    {
        top,
        centre,
        bottom
    };

    class ViewAligned
        : public View
    {
    public:
        template<class T>
        class WithView;

        ViewAligned(HorizontalAlignment horizontalAlignment = HorizontalAlignment::centre, VerticalAlignment verticalAlignment = VerticalAlignment::centre);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

        void SetSubView(View& newSubView);

        void SetHorizontalAlignment(HorizontalAlignment newHorizontalAlignment);
        void SetVerticalAlignment(VerticalAlignment newVerticalAlignment);
        void SetAlignment(HorizontalAlignment newHorizontalAlignment, VerticalAlignment newVerticalAlignment);

    protected:
        virtual void ViewRegionChanged() override;

    private:
        int16_t ComputeOriginX() const;
        int16_t ComputeOriginY() const;

    private:
        services::View* subView = nullptr;
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::centre;
        VerticalAlignment verticalAlignment = VerticalAlignment::centre;
    };

    template<class T>
    class ViewAligned::WithView
        : public ViewAligned
    {
    public:
        template<class... Args>
        WithView(Args&&... args)
            : subView(std::forward<Args>(args)...)
        {
            SetSubView(subView);
        }

        template<class... Args>
        WithView(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, Args&&... args)
            : ViewAligned(horizontalAlignment, verticalAlignment)
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
