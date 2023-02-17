#include "preview/interfaces/View.hpp"

namespace services
{
    void View::SetParent(View& newParent)
    {
        parent = &newParent;
    }

    void View::SetViewRegion(infra::Region newRegion)
    {
        Dirty(viewRegion | newRegion);
        viewRegion = newRegion;
        ViewRegionChanged();
    }

    void View::Move(infra::Point destination)
    {
        if (destination != viewRegion.Origin())
            SetViewRegion(infra::Region(destination, viewRegion.Size()));
    }

    void View::Resize(infra::Vector newSize)
    {
        if (newSize != viewRegion.Size())
            SetViewRegion(infra::Region(viewRegion.Origin(), newSize));
    }

    void View::ResetLayout()
    {
        ResetLayout(viewRegion.Size());
    }

    void View::ResetLayout(infra::Vector newSize)
    {
        auto oldRegion = viewRegion;
        ResetSize();
        if (newSize == oldRegion.Size())
        {
            viewRegion = oldRegion;
            // Trigger ViewRegionChanged without triggering Dirty so that subviews can be updated without changing this view's region
            ViewRegionChanged();
        }
        else
            Resize(newSize);
    }

    infra::Region View::ViewRegion() const
    {
        return viewRegion;
    }

    infra::Vector View::MinimumSize() const
    {
        return infra::Vector();
    }

    infra::Vector View::MaximumSize() const
    {
        return infra::Vector::Maximum();
    }

    infra::Region View::DrawRegion() const
    {
        return viewRegion;
    }

    void View::ResetSize()
    {}

    void View::ViewRegionChanged()
    {}

    void View::Dirty(infra::Region region)
    {
        if (parent != nullptr)
            parent->Dirty(region);
    }

    void View::ResizeWithoutTrigger(infra::Vector newSize)
    {
        viewRegion = infra::Region(viewRegion.Origin(), newSize);
    }

    bool IsHorizontal(Direction direction)
    {
        return direction == Direction::left || direction == Direction::right;
    }

    bool IsVertical(Direction direction)
    {
        return !IsHorizontal(direction);
    }
}
