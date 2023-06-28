#ifndef PREVIEW_LOW_POWER_STRATEGY_SDL_HPP
#define PREVIEW_LOW_POWER_STRATEGY_SDL_HPP

#include "infra/event/LowPowerEventDispatcher.hpp"
#include "infra/timer/TimerService.hpp"
#include "preview/sdl/SdlMouse.hpp"

namespace services
{
    class LowPowerStrategySdl
        : public infra::LowPowerStrategy
        , public SdlMouseSubject
    {
    public:
        LowPowerStrategySdl(infra::TimerService& timerService);

        void RequestExecution() override;
        void Idle(const infra::EventDispatcherWorker& eventDispatcher) override;

    private:
        infra::TimerService& timerService;
    };
}

#endif
