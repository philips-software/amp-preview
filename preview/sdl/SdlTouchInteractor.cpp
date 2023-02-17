#include "preview/sdl/SdlTouchInteractor.hpp"

namespace services
{
    SdlTouchInteractor::SdlTouchInteractor(SdlMouseSubject& subject, services::TouchRecipient& touchRecipient)
        : SdlMouseObserver(subject)
        , touchRecipient(touchRecipient)
    {}

    void SdlTouchInteractor::MouseDown(infra::Point point)
    {
        touching = true;
        touchRecipient.StartTouch(point);
    }

    void SdlTouchInteractor::MouseMoveTo(infra::Point point)
    {
        if (touching)
            touchRecipient.DragTo(point);
    }

    void SdlTouchInteractor::MouseUp()
    {
        touching = false;
        touchRecipient.StopTouch();
    }
}
