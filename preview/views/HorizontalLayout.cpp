#include "preview/views/HorizontalLayout.hpp"
#include "infra/util/ReverseRange.hpp"
#include <tuple>

namespace services
{
    HorizontalLayout::HorizontalLayout(infra::BoundedVector<detail::LayoutViewInfo>& views, uint8_t interMargin, uint8_t topMargin, uint8_t rightMargin, uint8_t bottomMargin, uint8_t leftMargin)
        : views(views)
        , interMargin(interMargin)
        , topMargin(topMargin)
        , rightMargin(rightMargin)
        , bottomMargin(bottomMargin)
        , leftMargin(leftMargin)
    {}

    infra::Vector HorizontalLayout::MinimumSize() const
    {
        infra::Vector result;

        for (auto& viewInfo : views)
        {
            if (viewInfo.view != nullptr)
            {
                infra::Vector minimumSize = viewInfo.view->MinimumSize();
                result.deltaY = std::max(result.deltaY, minimumSize.deltaY);

                if (viewInfo.alignment)
                    result.deltaY = std::max(result.deltaY, viewInfo.view->ViewRegion().Size().deltaY);

                if (viewInfo.proportion == 0)
                    result.deltaX = infra::RestrictedInt16Sum(result.deltaX, std::max(minimumSize.deltaX, viewInfo.view->ViewRegion().Size().deltaX));
                else
                    result.deltaX = infra::RestrictedInt16Sum(result.deltaX, minimumSize.deltaX);
            }
        }

        if (!views.empty())
            result.deltaX = infra::RestrictedInt16Sum(result.deltaX, static_cast<uint16_t>(views.size() - 1) * interMargin);

        return infra::RestrictedSum(result, infra::Vector(rightMargin + leftMargin, topMargin + bottomMargin));
    }

    infra::Vector HorizontalLayout::MaximumSize() const
    {
        infra::Vector result(0, std::numeric_limits<int16_t>::max());

        for (auto& viewInfo : views)
        {
            if (viewInfo.view != nullptr)
            {
                infra::Vector maximumSize = viewInfo.view->MaximumSize();

                if (!viewInfo.alignment)
                    result.deltaY = std::min(result.deltaY, maximumSize.deltaY);

                if (viewInfo.proportion == 0)
                    result.deltaX = infra::RestrictedInt16Sum(result.deltaX, viewInfo.view->ViewRegion().Size().deltaX);
                else
                    result.deltaX = infra::RestrictedInt16Sum(result.deltaX, maximumSize.deltaX);
            }
        }

        if (!views.empty())
            result.deltaX = infra::RestrictedInt16Sum(result.deltaX, static_cast<uint16_t>(views.size() - 1) * interMargin);
        else
            result.deltaX = std::numeric_limits<int16_t>::max();

        return infra::RestrictedSum(result, infra::Vector(rightMargin + leftMargin, topMargin + bottomMargin));
    }

    void HorizontalLayout::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        for (const auto& viewInfo : infra::MakeReverseRange(views))
            if (views[viewInfo.paintOrder].view != nullptr && !infra::Intersection(views[viewInfo.paintOrder].drawRegion, boundingRegion).Empty())
                views[viewInfo.paintOrder].view->Paint(canvas, boundingRegion);
    }

    infra::Region HorizontalLayout::DrawRegion() const
    {
        infra::Region result;

        for (auto& view : views)
            result = infra::Union(result, view.drawRegion);

        return result;
    }

    void HorizontalLayout::ResetSize()
    {
        for (auto& viewInfo : views)
            if (viewInfo.view != nullptr)
                viewInfo.view->ResetSize();

        ResizeWithoutTrigger(MinimumSize());
    }

    void HorizontalLayout::Add(View& view, uint16_t proportion, VerticalAlignment alignment)
    {
        views.push_back(detail::LayoutViewInfo{ &view, std::make_optional(alignment), proportion, static_cast<uint8_t>(views.size()) });
        view.SetParent(*this);
    }

    void HorizontalLayout::AddFill(View& view, uint16_t proportion)
    {
        views.push_back(detail::LayoutViewInfo{ &view, std::nullopt, proportion, static_cast<uint8_t>(views.size()) });
        view.SetParent(*this);
    }

    void HorizontalLayout::AddDummy(uint16_t proportion)
    {
        assert(proportion != 0);
        views.push_back(detail::LayoutViewInfo{ nullptr, std::nullopt, proportion, static_cast<uint8_t>(views.size()) });
    }

    void HorizontalLayout::BringToFront(View& view)
    {
        std::size_t viewIndex = std::distance(views.begin(), std::find_if(views.begin(), views.end(), [&view](const detail::LayoutViewInfo& info)
                                                                 {
                                                                     return info.view == &view;
                                                                 }));
        assert(viewIndex != views.size());

        std::size_t viewPaintOrder = std::distance(views.begin(), std::find_if(views.begin(), views.end(), [&view, this](const detail::LayoutViewInfo& info)
                                                                      {
                                                                          return views[info.paintOrder].view == &view;
                                                                      }));
        for (std::size_t i = viewPaintOrder; i != 0; --i)
            views[i].paintOrder = views[i - 1].paintOrder;
        views[0].paintOrder = static_cast<uint8_t>(viewIndex);
    }

    void HorizontalLayout::ViewRegionChanged()
    {
        infra::Region region(ViewRegion());
        infra::Region dirty;
        detail::HorizontalLayouter(views, interMargin).Layout(infra::Region(region.Top() + topMargin, region.Left() + leftMargin, region.Bottom() - bottomMargin, region.Right() - rightMargin), dirty);

        for (auto& viewInfo : views)
            if (viewInfo.view != nullptr)
                viewInfo.drawRegion = viewInfo.view->DrawRegion();
    }

    namespace detail
    {
        HorizontalLayouter::HorizontalLayouter(infra::BoundedVector<LayoutViewInfo>& views, uint8_t interMargin)
            : views(views)
            , interMargin(interMargin)
            , totalProportion(TotalProportion())
        {
            ClearViewInfos();
        }

        void HorizontalLayouter::Layout(infra::Region region, infra::Region& dirty)
        {
            region = AdjustOrientation(region);
            availableResizableX = region.Width() - static_cast<uint16_t>((views.size() - 1) * interMargin) - StaticNeededX();
            EliminateLimitedProportionalViews(region);
            AllocateViews(region, dirty);
        }

        infra::Point HorizontalLayouter::AdjustOrientation(infra::Point point) const
        {
            return point;
        }

        infra::Vector HorizontalLayouter::AdjustOrientation(infra::Vector vector) const
        {
            return vector;
        }

        infra::Region HorizontalLayouter::AdjustOrientation(infra::Region region) const
        {
            return region;
        }

        uint16_t HorizontalLayouter::TotalProportion() const
        {
            uint16_t result = 0;

            for (auto& viewInfo : views)
                result += viewInfo.proportion;

            return result;
        }

        void HorizontalLayouter::ClearViewInfos()
        {
            for (auto& viewInfo : views)
            {
                viewInfo.minimize = false;
                viewInfo.maximize = false;
            }
        }

        uint16_t HorizontalLayouter::StaticNeededX() const
        {
            uint16_t result = 0;

            for (auto& viewInfo : views)
                if (viewInfo.view != nullptr && viewInfo.proportion == 0)
                    result += AdjustOrientation(viewInfo.view->ViewRegion()).Width();

            return result;
        }

        void HorizontalLayouter::EliminateLimitedProportionalViews(infra::Region region)
        {
            while (totalProportion != 0)
            {
                uint16_t totalTooSmall = ComputeTooSmall();
                uint16_t totalTooBig = ComputeTooBig();

                if (totalTooSmall == 0 && totalTooBig == 0)
                    break;
                else if (totalTooBig > totalTooSmall)
                    FixateMaximumSizeViews();
                else
                    FixateMinimumSizeViews();
            }
        }

        uint16_t HorizontalLayouter::ComputeTooSmall() const
        {
            uint16_t totalAllottedX = 0;
            uint16_t runningProportion = 0;
            uint16_t totalTooSmall = 0;

            for (auto& viewInfo : views)
            {
                if (viewInfo.view != nullptr && viewInfo.proportion != 0 && !viewInfo.maximize && !viewInfo.minimize)
                {
                    infra::Vector minimumSize = AdjustOrientation(viewInfo.view->MinimumSize());
                    uint16_t allottedX = (availableResizableX - totalAllottedX) * viewInfo.proportion / (totalProportion - runningProportion);

                    if (minimumSize.deltaX > allottedX)
                        totalTooSmall += minimumSize.deltaX - allottedX;

                    totalAllottedX += allottedX; // Pretend that exactly the allotted size is used. Later on, the correction will be made.
                    runningProportion += viewInfo.proportion;
                }
            }

            return totalTooSmall;
        }

        uint16_t HorizontalLayouter::ComputeTooBig() const
        {
            uint16_t totalAllottedX = 0;
            uint16_t runningProportion = 0;
            uint16_t totalTooBig = 0;

            for (auto& viewInfo : views)
            {
                if (viewInfo.view != nullptr && viewInfo.proportion != 0 && !viewInfo.maximize && !viewInfo.minimize)
                {
                    infra::Vector maximumSize = AdjustOrientation(viewInfo.view->MaximumSize());
                    uint16_t allottedX = (availableResizableX - totalAllottedX) * viewInfo.proportion / (totalProportion - runningProportion);

                    if (maximumSize.deltaX < allottedX)
                        totalTooBig += allottedX - maximumSize.deltaX;

                    totalAllottedX += allottedX; // Pretend that exactly the allotted size is used. Later on, the correction will be made.
                    runningProportion += viewInfo.proportion;
                }
            }

            return totalTooBig;
        }

        void HorizontalLayouter::FixateMaximumSizeViews()
        {
            // The width needed to compensate for views that are too small to fill their allotted area is not enough to be compensated
            // by views that are too large, so fixate all views that are currently too small.

            uint16_t runningProportion = 0;
            uint16_t totalAllottedX = 0;

            for (auto& viewInfo : views)
            {
                if (viewInfo.view != nullptr && viewInfo.proportion != 0 && !viewInfo.maximize && !viewInfo.minimize)
                {
                    infra::Vector maximumSize = AdjustOrientation(viewInfo.view->MaximumSize());
                    uint16_t allottedX = (availableResizableX - totalAllottedX) * viewInfo.proportion / (totalProportion - runningProportion);

                    if (maximumSize.deltaX < allottedX)
                    {
                        viewInfo.maximize = true;
                        availableResizableX -= maximumSize.deltaX;
                        totalProportion -= viewInfo.proportion;
                    }
                    else
                        runningProportion += viewInfo.proportion;

                    totalAllottedX += allottedX;
                }
            }
        }

        void HorizontalLayouter::FixateMinimumSizeViews()
        {
            // The width needed to compensate for views that are too big to fill their allotted area is not enough to be compensated
            // by views that are too small, so fixate all views that are currently too big.

            uint16_t runningProportion = 0;
            uint16_t totalAllottedX = 0;

            for (auto& viewInfo : views)
            {
                if (viewInfo.view != nullptr && viewInfo.proportion != 0 && !viewInfo.maximize && !viewInfo.minimize)
                {
                    infra::Vector minimumSize = AdjustOrientation(viewInfo.view->MinimumSize());
                    uint16_t allottedX = (availableResizableX - totalAllottedX) * viewInfo.proportion / (totalProportion - runningProportion);

                    if (minimumSize.deltaX > allottedX)
                    {
                        viewInfo.minimize = true;
                        availableResizableX -= minimumSize.deltaX;
                        totalProportion -= viewInfo.proportion;
                    }
                    else
                        runningProportion += viewInfo.proportion;

                    totalAllottedX += allottedX;
                }
            }
        }

        void HorizontalLayouter::AllocateViews(infra::Region region, infra::Region& dirty)
        {
            infra::Point startOrigin = region.TopLeft();
            uint16_t runningProportion = 0;

            for (auto& viewInfo : views)
            {
                uint16_t viewTop = 0;
                int16_t viewHeight = 0;
                std::tie(viewTop, viewHeight) = ComputeVerticalPosition(region, viewInfo, startOrigin);
                infra::Region viewRegion(infra::Point(startOrigin.x, viewTop), infra::Vector(ComputeWidth(viewInfo, availableResizableX, runningProportion), viewHeight));

                startOrigin += infra::DeltaX(viewRegion.Width() + interMargin);
                if (viewInfo.view != nullptr)
                {
                    infra::Region newRegion = AdjustOrientation(viewRegion);
                    if (newRegion != viewInfo.view->ViewRegion())
                    {
                        dirty = dirty | viewInfo.view->ViewRegion() | newRegion;
                        viewInfo.view->SetViewRegion(newRegion);
                    }
                }
            }
        }

        std::pair<uint16_t, int16_t> HorizontalLayouter::ComputeVerticalPosition(infra::Region region, LayoutViewInfo& viewInfo, infra::Point startOrigin) const
        {
            if (viewInfo.view == nullptr)
                return std::make_pair(startOrigin.y, startOrigin.y);

            infra::Vector size = AdjustOrientation(viewInfo.view->ViewRegion()).Size();
            infra::Vector minimumSize = AdjustOrientation(viewInfo.view->MinimumSize());
            infra::Vector maximumSize = AdjustOrientation(viewInfo.view->MaximumSize());
            infra::Region viewRegion(startOrigin, infra::Vector(std::min(std::max(size.deltaX, minimumSize.deltaX), maximumSize.deltaX), std::min(std::max(size.deltaY, minimumSize.deltaY), maximumSize.deltaY)));

            uint16_t viewTop = 0;
            int16_t viewHeight = 0;

            if (viewInfo.alignment)
            {
                viewHeight = viewRegion.Height();
                switch (*viewInfo.alignment)
                {
                    case VerticalAlignment::top:
                        viewTop = region.Top();
                        break;
                    case VerticalAlignment::bottom:
                        viewTop = region.Bottom() - viewHeight;
                        viewRegion >>= infra::DeltaY(region.Height() - viewRegion.Height());
                        break;
                    case VerticalAlignment::centre:
                        viewTop = region.Top() + (region.Height() - viewHeight) / 2;
                        viewRegion >>= infra::DeltaY((region.Height() - viewRegion.Height()) / 2);
                        break;
                    default:
                        std::abort();
                }
            }
            else
            {
                viewHeight = std::max(std::min(region.Height(), maximumSize.deltaY), minimumSize.deltaY);
                viewTop = region.Top();
            }

            return std::make_pair(viewTop, viewHeight);
        }

        uint16_t HorizontalLayouter::ComputeWidth(const LayoutViewInfo& viewInfo, uint16_t& availableResizableX, uint16_t& runningProportion) const
        {
            if (viewInfo.proportion == 0 || viewInfo.maximize || viewInfo.minimize)
            {
                if (viewInfo.maximize)
                {
                    infra::Vector maximumSize = AdjustOrientation(viewInfo.view->MaximumSize());
                    return maximumSize.deltaX;
                }
                else if (viewInfo.minimize)
                {
                    infra::Vector minimumSize = AdjustOrientation(viewInfo.view->MinimumSize());
                    return minimumSize.deltaX;
                }
                else
                {
                    infra::Vector minimumSize = AdjustOrientation(viewInfo.view->MinimumSize());
                    infra::Vector maximumSize = AdjustOrientation(viewInfo.view->MaximumSize());
                    return std::min(std::max(AdjustOrientation(viewInfo.view->ViewRegion()).Width(), minimumSize.deltaX), maximumSize.deltaX);
                }
            }
            else
            {
                uint16_t allottedX = availableResizableX * viewInfo.proportion / (totalProportion - runningProportion);
                availableResizableX -= allottedX;
                runningProportion += viewInfo.proportion;
                return allottedX;
            }
        }
    }
}
