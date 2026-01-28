#include "preview/sdl/LowPowerStrategySdl.hpp"
#include "SDL3/SDL.h"

namespace services
{
    LowPowerStrategySdl::LowPowerStrategySdl(infra::TimerService& timerService)
        : timerService(timerService)
    {}

    void LowPowerStrategySdl::RequestExecution()
    {}

    void LowPowerStrategySdl::Idle(const infra::EventDispatcherWorker& eventDispatcher)
    {
        SDL_Event event;
        if (SDL_WaitEventTimeout(&event, static_cast<int>(std::min<uint64_t>(std::max<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(timerService.NextTrigger() - timerService.Now()).count(), 0), 10000))) != 0)
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    std::exit(0);
                case SDL_EVENT_MOUSE_MOTION:
                {
                    SDL_MouseMotionEvent& mouseEvent = reinterpret_cast<SDL_MouseMotionEvent&>(event);
                    NotifyObservers([&mouseEvent](SdlMouseObserver& observer)
                        {
                            observer.MouseMoveTo(infra::Point(mouseEvent.x, mouseEvent.y));
                        });
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    SDL_MouseButtonEvent& mouseEvent = reinterpret_cast<SDL_MouseButtonEvent&>(event);
                    if (mouseEvent.button == 1)
                        NotifyObservers([&mouseEvent](SdlMouseObserver& observer)
                            {
                                observer.MouseDown(infra::Point(mouseEvent.x, mouseEvent.y));
                            });
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    SDL_MouseButtonEvent& mouseEvent = reinterpret_cast<SDL_MouseButtonEvent&>(event);
                    if (mouseEvent.button == 1)
                        NotifyObservers([](SdlMouseObserver& observer)
                            {
                                observer.MouseUp();
                            });
                    break;
                }
            }
    }
}
