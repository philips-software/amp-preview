#include "preview/views/VerticalLayout.hpp"
#include "infra/util/ReverseRange.hpp"

namespace services
{
    VerticalLayout::VerticalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, uint8_t interMargin, uint8_t topMargin, uint8_t rightMargin, uint8_t bottomMargin, uint8_t leftMargin)
        : views(views)
        , interMargin(interMargin)
        , topMargin(topMargin)
        , rightMargin(rightMargin)
        , bottomMargin(bottomMargin)
        , leftMargin(leftMargin)
    {}

    infra::Vector VerticalLayout::MinimumSize() const
    {
        infra::Vector result;

        for (auto& viewInfo : views)
        {
            if (viewInfo.view != nullptr)
            {
                infra::Vector minimumSize = viewInfo.view->MinimumSize();
                result.deltaX = std::max(result.deltaX, minimumSize.deltaX);

                if (viewInfo.alignment)
                    result.deltaX = std::max(result.deltaX, viewInfo.view->ViewRegion().Size().deltaX);

                if (viewInfo.proportion == 0)
                    result.deltaY = infra::RestrictedInt16Sum(result.deltaY, std::max(minimumSize.deltaY, viewInfo.view->ViewRegion().Size().deltaY));
                else
                    result.deltaY = infra::RestrictedInt16Sum(result.deltaY, minimumSize.deltaY);
            }
        }

        if (!views.empty())
            result.deltaY = infra::RestrictedInt16Sum(result.deltaY, static_cast<uint16_t>(views.size() - 1) * interMargin);

        return infra::RestrictedSum(result, infra::Vector(rightMargin + leftMargin, topMargin + bottomMargin));
    }

    infra::Vector VerticalLayout::MaximumSize() const
    {
        infra::Vector result(std::numeric_limits<int16_t>::max(), 0);

        for (auto& viewInfo : views)
        {
            if (viewInfo.view != nullptr)
            {
                infra::Vector maximumSize = viewInfo.view->MaximumSize();

                if (!viewInfo.alignment)
                    result.deltaX = std::min(result.deltaX, maximumSize.deltaX);

                if (viewInfo.proportion == 0)
                    result.deltaY = infra::RestrictedInt16Sum(result.deltaY, viewInfo.view->ViewRegion().Size().deltaY);
                else
                    result.deltaY = infra::RestrictedInt16Sum(result.deltaY, maximumSize.deltaY);
            }
        }

        if (!views.empty())
            result.deltaY = infra::RestrictedInt16Sum(result.deltaY, static_cast<uint16_t>(views.size() - 1) * interMargin);
        else
            result.deltaY = std::numeric_limits<int16_t>::max();

        return infra::RestrictedSum(result, infra::Vector(rightMargin + leftMargin, topMargin + bottomMargin));
    }

    void VerticalLayout::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        for (auto& viewInfo : infra::MakeReverseRange(views))
            if (views[viewInfo.paintOrder].view != nullptr)
                views[viewInfo.paintOrder].view->Paint(canvas, boundingRegion);
    }

    void VerticalLayout::ResetSize()
    {
        for (auto& viewInfo : views)
            if (viewInfo.view != nullptr)
                viewInfo.view->ResetSize();

        ResizeWithoutTrigger(MinimumSize());
    }

    void VerticalLayout::Add(View& view, uint16_t proportion, HorizontalAlignment alignment)
    {
        views.push_back(detail::LayoutViewInfo{ &view, infra::MakeOptional(Flip(alignment)), proportion, static_cast<uint8_t>(views.size()) });
        view.SetParent(*this);
    }

    void VerticalLayout::AddFill(View& view, uint16_t proportion)
    {
        views.push_back(detail::LayoutViewInfo{ &view, infra::none, proportion, static_cast<uint8_t>(views.size()) });
        view.SetParent(*this);
    }

    void VerticalLayout::AddDummy(uint16_t proportion)
    {
        assert(proportion != 0);
        views.push_back(detail::LayoutViewInfo{ nullptr, infra::none, proportion, static_cast<uint8_t>(views.size()) });
    }

    void VerticalLayout::BringToFront(View& view)
    {
        std::size_t viewIndex = std::distance(views.begin(), std::find_if(views.begin(), views.end(), [&view](const detail::LayoutViewInfo& info)
                                                                 { return info.view == &view; }));
        assert(viewIndex != views.size());

        std::size_t viewPaintOrder = std::distance(views.begin(), std::find_if(views.begin(), views.end(), [&view, this](const detail::LayoutViewInfo& info)
                                                                      { return views[info.paintOrder].view == &view; }));
        for (std::size_t i = viewPaintOrder; i != 0; --i)
            views[i].paintOrder = views[i - 1].paintOrder;
        views[0].paintOrder = static_cast<uint8_t>(viewIndex);
    }

    void VerticalLayout::ViewRegionChanged()
    {
        infra::Region region(ViewRegion());
        infra::Region dirty;
        detail::VerticalLayouter(views, interMargin).Layout(infra::Region(region.Top() + topMargin, region.Left() + leftMargin, region.Bottom() - bottomMargin, region.Right() - rightMargin), dirty);
    }

    VerticalAlignment VerticalLayout::Flip(HorizontalAlignment alignment) const
    {
        switch (alignment)
        {
            case HorizontalAlignment::left:
                return VerticalAlignment::top;
            case HorizontalAlignment::centre:
                return VerticalAlignment::centre;
            case HorizontalAlignment::right:
                return VerticalAlignment::bottom;
            default:
                std::abort();
        }
    }

    namespace detail
    {
        infra::Point VerticalLayouter::AdjustOrientation(infra::Point point) const
        {
            return infra::Flip(point);
        }

        infra::Vector VerticalLayouter::AdjustOrientation(infra::Vector vector) const
        {
            return infra::Flip(vector);
        }

        infra::Region VerticalLayouter::AdjustOrientation(infra::Region region) const
        {
            return infra::Flip(region);
        }
    }
}
