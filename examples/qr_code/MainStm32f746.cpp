#include "generated/stm32fxxx/PinoutTableDefault.hpp"
#include "hal_st/stm32fxxx/DefaultClockDiscoveryF746G.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/I2cStm.hpp"
#include "hal_st/stm32fxxx/SdRamStm.hpp"
#include "hal_st/stm32fxxx/SystemTickTimerService.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "preview/interfaces/QrCode.hpp"
#include "preview/interfaces/ViewPainterDoubleBufferDisplay.hpp"
#include "preview/interfaces/ViewRepainter.hpp"
#include "preview/stm32fxxx/BitmapPainterStm.hpp"
#include "preview/stm32fxxx/LcdStm.hpp"
#include "preview/views/ViewBitmap.hpp"
#include "services/util/DebugLed.hpp"

unsigned int hse_value = 25000000;

namespace main_
{
    struct Lcd
    {
        hal::MultiGpioPinStm sdRamPins{ hal::stm32f7discoveryFmcPins };
        hal::SdRamStm sdRam{ sdRamPins, hal::stm32f7discoverySdRamConfig };

        hal::MultiGpioPinStm lcdPins{ hal::stm32f7discoveryLcdPins };
        hal::GpioPinStm displayEnable{ hal::Port::I, 12 };
        hal::GpioPinStm backlightEnable{ hal::Port::K, 3 };
        uint32_t bufferSize{ infra::Bitmap::BufferSize(hal::stm32f7discoveryLcdConfig.width, hal::stm32f7discoveryLcdConfig.height, hal::stm32f7discoveryLcdConfig.pixelFormat) };
        infra::ByteRange lcdBuffer0{ infra::Head(sdRam.Memory(), bufferSize) };
        infra::ByteRange lcdBuffer1{ infra::Head(infra::DiscardHead(sdRam.Memory(), bufferSize), bufferSize) };
        infra::Bitmap bitmap0{ lcdBuffer0, infra::Vector(480, 272), infra::PixelFormat::rgb565 };
        infra::Bitmap bitmap1{ lcdBuffer1, infra::Vector(480, 272), infra::PixelFormat::rgb565 };
        hal::LcdStmDoubleBuffer display{ lcdPins, displayEnable, backlightEnable, lcdBuffer0, lcdBuffer1, hal::stm32f7discoveryLcdConfig };
    };
}

int main()
{
    HAL_Init();
    ConfigureDefaultClockDiscoveryF746G();

    static hal::InterruptTable::WithStorage<128> interruptTable;
    static infra::EventDispatcherWithWeakPtr::WithSize<50> eventDispatcher;
    static hal::GpioStm gpio(hal::pinoutTableDefaultStm);
    static hal::SystemTickTimerService systemTick;

    static main_::Lcd lcd;
    static hal::BitmapPainterStm bitmapPainter;
    static services::ViewPainterDoubleBufferDisplay painter(lcd.display, bitmapPainter);

    services::QrCode<3, services::QrCodeEcc::low> qrcode("https://github.com/philips-software/amp-preview");
    services::ViewBitmap viewBitmap(qrcode);

    static services::ViewRepainterPaintWhenDirty repainter(painter, viewBitmap);
    viewBitmap.ResetLayout(lcd.display.DisplaySize());

    eventDispatcher.Run();
    __builtin_unreachable();
}
