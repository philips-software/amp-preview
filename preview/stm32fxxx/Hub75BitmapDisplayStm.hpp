#ifndef PREVIEW_HUB75_BITMAP_DISPLAY_STM_HPP
#define PREVIEW_HUB75_BITMAP_DISPLAY_STM_HPP

#include "hal/interfaces/Gpio.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerPwmStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/WithStorage.hpp"
#include "preview/interfaces/Hub75Bitmap.hpp"
#include "preview/interfaces/ViewPainter.hpp"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

namespace hal
{
    class Hub75BitmapDisplayStm
        : public services::ViewPainter
    {
    public:
        template<int32_t width = 64, int32_t height = 32>
        struct WithDimensions
            : infra::WithStorage<Hub75BitmapDisplayStm, infra::Hub75Bitmap::WithDimensions<width, height>>
        {
            WithDimensions(hal::DmaStm& dma, infra::Vector panelSize = infra::Vector(64, 32))
                : infra::WithStorage<Hub75BitmapDisplayStm, infra::Hub75Bitmap::WithDimensions<width, height>>(std::in_place, panelSize, dma)
            {}
        };

        Hub75BitmapDisplayStm(infra::Hub75Bitmap& bitmap, hal::DmaStm& dma);

        virtual void Paint(services::View& view, infra::Region region, infra::Function<void()> onDone) override;

    protected:
        virtual void SetAddress();

    private:
        void DisplayStep();
        void TransferComplete();

    protected:
        infra::Hub75Bitmap& bitmap;
        uint8_t blockCount = 16;
        uint16_t blockSize = static_cast<uint16_t>(bitmap.buffer.size() / blockCount);
        uint8_t block = 0;

    private:
        hal::DmaStm& dma;

        hal::GpioPinStm gp0{ hal::Port::D, 0 };
        hal::GpioPinStm gp1{ hal::Port::D, 1 };
        hal::GpioPinStm gp2{ hal::Port::D, 2 };
        hal::GpioPinStm gp3{ hal::Port::D, 3 };
        hal::GpioPinStm gp4{ hal::Port::D, 4 };
        hal::GpioPinStm gp5{ hal::Port::D, 5 };
        hal::GpioPinStm gp6{ hal::Port::D, 6 };
        hal::GpioPinStm gp7{ hal::Port::D, 7 };

        hal::GpioPinStm ga{ hal::Port::E, 4 };
        hal::GpioPinStm gb{ hal::Port::F, 0 };
        hal::GpioPinStm gc{ hal::Port::F, 1 };
        hal::GpioPinStm gd{ hal::Port::F, 2 };
        hal::GpioPinStm goe{ hal::Port::E, 9 };
        hal::GpioPinStm glat{ hal::Port::F, 10 };

        hal::OutputPin p0{ gp0 };
        hal::OutputPin p1{ gp1 };
        hal::OutputPin p2{ gp2 };
        hal::OutputPin p3{ gp3 };
        hal::OutputPin p4{ gp4 };
        hal::OutputPin p5{ gp5 };
        hal::OutputPin p6{ gp6 };
        hal::OutputPin p7{ gp7 };

        hal::OutputPin a{ ga };
        hal::OutputPin b{ gb };
        hal::OutputPin c{ gc };
        hal::OutputPin d{ gd };
        hal::OutputPin lat{ glat };

        hal::TimerPwmLazy::WithChannels<1> timerPwmOutputEnable{ 1, hal::TimerBaseStm::Timing{ 4, 8 } };

        hal::PwmChannelGpio& outputEnable{ [this]() -> hal::PwmChannelGpio&
            {
                timerPwmOutputEnable.ConfigureChannel(1, goe);
                return timerPwmOutputEnable.Channel(1);
            }() };

        // 108MHz transmits
        hal::FreeRunningTimerStm timerTransmit{ 8, hal::TimerBaseStm::Timing{ 1, 1 }, hal::FreeRunningTimerStm::Config{ hal::FreeRunningTimerStm::CounterMode::up } };
        // For some reason Debug builds need a lower transmission speed
        // hal::FreeRunningTimerStm timerTransmit{ 8, hal::TimerBaseStm::Timing{ 1, 4 }, hal::FreeRunningTimerStm::Config{ hal::FreeRunningTimerStm::CounterMode::up } };

        // // 4kHz display updates in Debug
        // hal::TimerWithInterruptStm timerDisplay{ 2, hal::TimerBaseStm::Timing{ 216 / 4, 250 } };
        // Even lower display updates in Debug
        // hal::TimerWithInterruptStm timerDisplay{ 2, hal::TimerBaseStm::Timing{ 216 / 4, 1000 } };

        // 5kHz display updates
        hal::TimerWithInterruptStm timerDisplay{ 2, hal::TimerBaseStm::Timing{ 216 / 4, 200 } };

        hal::DmaStm::TransmitStream stream{ dma, hal::DmaChannelId{ 2, 1, 7 } }; // Stream/channel for TIM8 Update
        hal::TransmitDmaChannel channel{ stream, &GPIOD->ODR, 1, [this]()
            {
                TransferComplete();
            },
            hal::DmaStm::StreamInterruptHandler::immediate };
    };

    class Hub75EBitmapDisplayStm
        : public Hub75BitmapDisplayStm
    {
    public:
        template<int32_t width = 128, int32_t height = 64>
        struct WithDimensions
            : infra::WithStorage<Hub75EBitmapDisplayStm, infra::Hub75Bitmap::WithDimensions<width, height>>
        {
            WithDimensions(hal::DmaStm& dma, infra::Vector panelSize = infra::Vector(128, 64))
                : infra::WithStorage<Hub75EBitmapDisplayStm, infra::Hub75Bitmap::WithDimensions<width, height>>(std::in_place, panelSize, dma)
            {}
        };

        Hub75EBitmapDisplayStm(infra::Hub75Bitmap& bitmap, hal::DmaStm& dma);

    protected:
        void SetAddress() override;

    private:
        hal::GpioPinStm ge{ hal::Port::F, 3 };
        hal::OutputPin e{ ge };
    };
}

#endif
