#ifndef PREVIEW_VIEW_RESTRICT_PAINT_HPP
#define PREVIEW_VIEW_RESTRICT_PAINT_HPP

#include "preview/interfaces/View.hpp"

namespace services
{
    template<class T>
    class RestrictPaint
        : public T
    {
    public:
        using T::T;

        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
    };

    template<class T>
    void RestrictPaint<T>::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        T::Paint(canvas, boundingRegion & T::ViewRegion());
    }
}

#endif
