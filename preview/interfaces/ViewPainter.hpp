#ifndef PREVIEW_VIEW_PAINTER_HPP
#define PREVIEW_VIEW_PAINTER_HPP

#include "infra/util/Function.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewPainter
    {
    protected:
        ViewPainter() = default;
        ViewPainter(const ViewPainter& other) = delete;
        ViewPainter& operator=(const ViewPainter& other) = delete;
        ~ViewPainter() = default;

    public:
        virtual void Paint(View& view, infra::Region region, infra::Function<void()> onDone) = 0;
    };
}

#endif
