#ifndef PREVIEW_SDL_MOUSE_HPP
#define PREVIEW_SDL_MOUSE_HPP

#include "infra/util/Observer.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace services
{
    class SdlMouseSubject;

    class SdlMouseObserver
        : public infra::Observer<SdlMouseObserver, SdlMouseSubject>
    {
    public:
        using infra::Observer<SdlMouseObserver, SdlMouseSubject>::Observer;

        virtual void MouseDown(infra::Point point) = 0;
        virtual void MouseMoveTo(infra::Point point) = 0;
        virtual void MouseUp() = 0;
    };

    class SdlMouseSubject
        : public infra::Subject<SdlMouseObserver>
    {};
}

#endif
