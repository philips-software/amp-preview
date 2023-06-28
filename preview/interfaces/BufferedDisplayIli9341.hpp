#ifndef PREVIEW_BUFFERED_DISPLAY_ILI9341_HPP
#define PREVIEW_BUFFERED_DISPLAY_ILI9341_HPP

#include "hal/interfaces/Gpio.hpp"
#include "hal/interfaces/Spi.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/Sequencer.hpp"
#include "preview/interfaces/BufferedDisplay.hpp"

namespace services
{
    class BufferedDisplayIli9341
        : public hal::BufferedDisplay
    {
    public:
        BufferedDisplayIli9341(hal::SpiMaster& spi, hal::GpioPin& dataCommandPin, hal::CommunicationConfigurator& spiWidth16Configurator);

    public:
        infra::Region DisplayRegion() const override;
        infra::PixelFormat PixelFormat() const override;
        void DrawBitmap(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone) override;

    private:
        void InitDisplay();
        void SendBitmap();
        void SendCommand(infra::ConstByteRange command);
        void SendData(infra::ConstByteRange data);

    private:
        hal::SpiMaster& spi;
        hal::OutputPin dataCommandPin;
        hal::CommunicationConfigurator& spiWidth16Configurator;

        infra::Sequencer sequencer;
        infra::TimerSingleShot delayTimer;
        infra::Function<void()> onDone;

        uint8_t command;
        uint16_t x0;
        uint16_t x1;
        uint16_t y0;
        uint16_t y1;
        infra::ConstByteRange bitmapBuffer;
    };
}

#endif
