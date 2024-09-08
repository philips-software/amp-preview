#include "examples/clicking_scrolling/TouchViewClickingScrolling.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/event/LowPowerEventDispatcher.hpp"
#include "preview/interfaces/QRCode.hpp"
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

    QRCode qrcode;
    uint8_t* qrcodeBytes = reinterpret_cast<uint8_t*>(malloc(qrcode_getBufferSize(3)));
    qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, "HELLO WORLD");

    infra::Bitmap::BlackAndWhite<29, 29> qrCodeBitmap;

    for (uint8_t y = 0; y < qrcode.size; y++)
        for (uint8_t x = 0; x < qrcode.size; x++)
            qrCodeBitmap.SetBlackAndWhitePixel(infra::Point(x, y), qrcode_getModule(&qrcode, x, y) == 0);

    services::ViewBitmap viewBitmap(qrCodeBitmap);

    hal::DirectDisplaySdl display(infra::Vector(480, 272));
    services::ViewPainterDirectDisplay painter(display);

    application::TouchViewClickingScrolling touchView;
    services::SdlTouchInteractor touchInteractor(lowPowerStrategy, touchView);

    services::ViewRepainterPaintWhenDirty repainter(painter, viewBitmap);
    viewBitmap.ResetLayout(display.Size());
    touchView.GetView().ResetLayout(display.Size());

    eventDispatcher.Run();

    return 0;
}
