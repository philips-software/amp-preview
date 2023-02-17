#include "examples/clicking_scrolling/TouchViewClickingScrolling.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/event/LowPowerEventDispatcher.hpp"
#include "preview/interfaces/ViewPainterDirectDisplay.hpp"
#include "preview/interfaces/ViewRepainter.hpp"
#include "preview/sdl/DirectDisplaySdl.hpp"
#include "preview/sdl/LowPowerStrategySdl.hpp"
#include "preview/sdl/SdlTouchInteractor.hpp"
#define NOMINMAX
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hal::TimerServiceGeneric timerService;
    services::LowPowerStrategySdl lowPowerStrategy(timerService);
    infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher(lowPowerStrategy);

    hal::DirectDisplaySdl display(infra::Vector(480, 272));
    services::ViewPainterDirectDisplay painter(display);

    application::TouchViewClickingScrolling touchView;
    services::SdlTouchInteractor touchInteractor(lowPowerStrategy, touchView);

    services::ViewRepainterPaintWhenDirty repainter(painter, touchView.GetView());
    touchView.GetView().ResetLayout(display.Size());

    eventDispatcher.Run();

    return 0;
}
