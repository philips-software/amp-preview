#ifndef PREVIEW_SDL_TOUCH_INTERACTOR_HPP
#define PREVIEW_SDL_TOUCH_INTERACTOR_HPP

#include "infra/util/Observer.hpp"
#include "preview/sdl/SdlMouse.hpp"
#include "preview/touch/TouchRecipient.hpp"

namespace services
{
    class SdlTouchInteractor
        : public SdlMouseObserver
    {
    public:
        SdlTouchInteractor(SdlMouseSubject& subject, services::TouchRecipient& touchRecipient);

        void MouseDown(infra::Point point) override;
        void MouseMoveTo(infra::Point point) override;
        void MouseUp() override;

    private:
        services::TouchRecipient& touchRecipient;
        bool touching = false;
    };
}

#endif
