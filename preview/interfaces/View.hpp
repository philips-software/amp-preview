#ifndef PREVIEW_VIEW_HPP
#define PREVIEW_VIEW_HPP

#include "preview/interfaces/Canvas.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace services
{
    class View
    {
    protected:
        View() = default;
        View(const View& other) = delete;
        View& operator=(const View& other) = delete;
        ~View() = default;

    public:
        void SetParent(View& newParent);
        void SetViewRegion(infra::Region newRegion); // region.Size() must always be between MinimumSize() and MaximumSize()
        void Move(infra::Point destination);
        void Resize(infra::Vector newSize);
        void ResetLayout();
        void ResetLayout(infra::Vector newSize);
        infra::Region ViewRegion() const;

        virtual infra::Vector MinimumSize() const;
        virtual infra::Vector MaximumSize() const;

        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) = 0;
        virtual infra::Region DrawRegion() const;

        virtual void ResetSize(); // Only called by ResetLayout

    protected:
        virtual void ViewRegionChanged();
        virtual void Dirty(infra::Region region);
        void ResizeWithoutTrigger(infra::Vector newSize);

    private:
        infra::Region viewRegion;
        View* parent = nullptr;
    };

    enum class Direction
    {
        left,
        right,
        up,
        down
    };

    bool IsHorizontal(Direction direction);
    bool IsVertical(Direction direction);
}

#endif
