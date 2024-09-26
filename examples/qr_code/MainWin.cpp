#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/event/LowPowerEventDispatcher.hpp"
#include "preview/interfaces/QrCode.hpp"
#include "preview/interfaces/ViewPainterDirectDisplay.hpp"
#include "preview/interfaces/ViewRepainter.hpp"
#include "preview/sdl/DirectDisplaySdl.hpp"
#include "preview/sdl/LowPowerStrategySdl.hpp"
#include "preview/sdl/SdlTouchInteractor.hpp"
#include "preview/views/ViewBitmap.hpp"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hal::TimerServiceGeneric timerService;
    services::LowPowerStrategySdl lowPowerStrategy(timerService);
    infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher(lowPowerStrategy);

    services::QrCode<4, services::QrCodeEcc::low> qrcode("https://github.com/philips-software/amp-preview/pull/191");
    services::ViewBitmap viewBitmap(qrcode);

    hal::DirectDisplaySdl display(infra::Vector(480, 272));
    services::ViewPainterDirectDisplay painter(display);

    services::ViewRepainterPaintWhenDirty repainter(painter, viewBitmap);
    viewBitmap.ResetLayout(display.Size());

    eventDispatcher.Run();

    return 0;
}
