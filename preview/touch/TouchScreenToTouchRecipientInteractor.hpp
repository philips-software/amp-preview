#ifndef PREVIEW_TOUCH_SCREEN_TO_TOUCH_RECIPIENT_INTERACTOR_HPP
#define PREVIEW_TOUCH_SCREEN_TO_TOUCH_RECIPIENT_INTERACTOR_HPP

#include "infra/timer/Timer.hpp"
#include "infra/util/BoundedDeque.hpp"
#include "preview/touch/TouchRecipient.hpp"
#include "preview/touch/TouchScreen.hpp"

namespace services
{
    class TouchScreenToTouchRecipientInteractor
    {
    public:
        TouchScreenToTouchRecipientInteractor(services::TouchScreen& touchScreen, services::TouchRecipient& touchRecipient, infra::Vector displaySize, infra::Vector touchOffset);

    private:
        void MeasureTouch();
        void HandleTouch(infra::Point position);
        void HandleNoTouch();

    private:
        services::TouchScreen& touchScreen;
        services::TouchRecipient& touchRecipient;
        infra::Vector displaySize;
        infra::Vector touchOffset;
        infra::TimerRepeating measureTimer;
        bool measurementBusy = false;
        bool touching = false;
        uint32_t successiveNoTouches = 0;
        infra::BoundedDeque<infra::Point>::WithMaxSize<3> lastTouches;
    };
}

#endif
