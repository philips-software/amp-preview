#ifndef PREVIEW_LCD_STM_HPP
#define PREVIEW_LCD_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "preview/interfaces/DoubleBufferDisplay.hpp"
#include "preview/interfaces/MultiBufferDisplay.hpp"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

namespace hal
{
    class LcdStm
    {
    public:
        struct Config
        {
            uint16_t width;
            uint16_t height;
            infra::PixelFormat pixelFormat;

            uint32_t horizontalSync;
            uint32_t horizontalFrontPorch;
            uint32_t horizontalBackPorch;
            uint32_t verticalSync;
            uint32_t verticalFrontPorch;
            uint32_t verticalBackPorch;

            uint32_t frequencyDivisor;
        };

        LcdStm(hal::MultiGpioPinStm& lcdPins, hal::GpioPin& displayEnable, hal::GpioPin& backlightEnable, infra::ByteRange buffer0, infra::ByteRange buffer1, const Config& config);

        void DisplayOn();
        void DisplayOff();
        infra::Vector DisplaySize() const;
        void SwapLayers(const infra::Function<void()>& onDone);
        void SwapLayers(infra::ConstByteRange newActiveBuffer, const infra::Function<void()>& onDone);

    private:
        struct Layer
        {
            LTDC_Layer_TypeDef& layer;
            infra::ByteRange buffer;
            Layer& other;
        };

    private:
        void InitLtdc(const Config& config);
        void InitLayers(const Config& config);
        void OnReload();

    private:
        hal::MultiPeripheralPinStm lcdPins;
        hal::OutputPin displayEnable;
        hal::OutputPin backlightEnable;

        LTDC_HandleTypeDef ltdcHandle;

        infra::Vector displaySize;

        ImmediateInterruptHandler reload;

        std::array<Layer, 2> layers;
        Layer* currentLayer{ &layers[0] };

        infra::Function<void()> onDone;
    };

    class LcdStmDoubleBuffer
        : public LcdStm
        , public hal::DoubleBufferDisplay
    {
    public:
        LcdStmDoubleBuffer(hal::MultiGpioPinStm& lcdPins, hal::GpioPin& displayEnable, hal::GpioPin& backlightEnable, infra::ByteRange buffer0, infra::ByteRange buffer1, const Config& config);

        virtual void SwapLayers(const infra::Function<void()>& onDone) override;
        virtual infra::Bitmap& DrawingBitmap() override;
        virtual const infra::Bitmap& ViewingBitmap() const override;

    private:
        struct Layer
        {
            infra::Bitmap& bitmap;
            Layer& other;
        };

    private:
        infra::Bitmap bitmap0;
        infra::Bitmap bitmap1;

        std::array<Layer, 2> layers{ { { bitmap0, layers[1] }, { bitmap1, layers[0] } } };
        Layer* currentLayer{ &layers[0] };
    };

    class LcdStmMultiBuffer
        : public LcdStm
        , public MultiBufferDisplay
    {
    public:
        using LcdStm::LcdStm;

        virtual void SetBitmap(const infra::Bitmap& bitmap, const infra::Function<void()>& onDone) override;
    };

    inline constexpr LcdStm::Config stm32f7discoveryLcdConfig = { 480, 272, infra::PixelFormat::rgb565, 41, 32, 13, 10, 2, 2, 5 };
    inline constexpr std::array<std::pair<hal::Port, uint8_t>, 28> stm32f7discoveryLcdPins = { {
        { hal::Port::E, 4 },
        { hal::Port::G, 12 },
        { hal::Port::I, 9 },
        { hal::Port::I, 10 },
        { hal::Port::I, 14 },
        { hal::Port::I, 15 },
        { hal::Port::J, 0 },
        { hal::Port::J, 1 },
        { hal::Port::J, 2 },
        { hal::Port::J, 3 },
        { hal::Port::J, 4 },
        { hal::Port::J, 5 },
        { hal::Port::J, 6 },
        { hal::Port::J, 7 },
        { hal::Port::J, 8 },
        { hal::Port::J, 9 },
        { hal::Port::J, 10 },
        { hal::Port::J, 11 },
        { hal::Port::J, 13 },
        { hal::Port::J, 14 },
        { hal::Port::J, 15 },
        { hal::Port::K, 0 },
        { hal::Port::K, 1 },
        { hal::Port::K, 2 },
        { hal::Port::K, 4 },
        { hal::Port::K, 5 },
        { hal::Port::K, 6 },
        { hal::Port::K, 7 },
    } };
}

#endif
