#include "preview/stm32fxxx/LcdStm.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace hal
{
    LcdStm::LcdStm(hal::MultiGpioPinStm& lcdPins, hal::GpioPin& displayEnable, hal::GpioPin& backlightEnable, infra::ByteRange buffer0, infra::ByteRange buffer1, const Config& config)
        : lcdPins(lcdPins, hal::PinConfigTypeStm::lcd, 0)
        , displayEnable(displayEnable)
        , backlightEnable(backlightEnable)
        , displaySize(config.width, config.height)
        , reload(LTDC_IRQn, [this]()
              {
                  OnReload();
              })
        , layers{ { { *LTDC_Layer1, buffer0, layers[1] }, { *LTDC_Layer2, buffer1, layers[0] } } }
    {
        assert(buffer0.size() == infra::Bitmap::BufferSize(config.width, config.height, config.pixelFormat));
        assert(buffer1.size() == infra::Bitmap::BufferSize(config.width, config.height, config.pixelFormat));

        InitLtdc(config);
        InitLayers(config);

        DisplayOn();
    }

    void LcdStm::DisplayOn()
    {
        LTDC->GCR |= LTDC_GCR_LTDCEN;

        displayEnable.Set(true);
        backlightEnable.Set(true);
    }

    void LcdStm::DisplayOff()
    {
        LTDC->GCR &= ~LTDC_GCR_LTDCEN;

        displayEnable.Set(false);
        backlightEnable.Set(false);
    }

    infra::Vector LcdStm::DisplaySize() const
    {
        return displaySize;
    }

    void LcdStm::SwapLayers(const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;
        LTDC->ICR |= LTDC_ICR_CRRIF;
        LTDC->IER |= LTDC_IER_RRIE;

        currentLayer->layer.CACR = 255;
        currentLayer->other.layer.CACR = 0;
        currentLayer = &currentLayer->other;
        LTDC->SRCR = LTDC_SRCR_VBR;
    }

    void LcdStm::SwapLayers(infra::ConstByteRange newActiveBuffer, const infra::Function<void()>& onDone)
    {
        assert(newActiveBuffer.size() == infra::Bitmap::BufferSize(ltdcHandle.LayerCfg->ImageWidth, ltdcHandle.LayerCfg->ImageHeight, infra::PixelFormat::rgb565));
        currentLayer->layer.CFBAR = reinterpret_cast<uint32_t>(newActiveBuffer.begin());

        SwapLayers(onDone);
    }

    void LcdStm::InitLtdc(const Config& config)
    {
        RCC_PeriphCLKInitTypeDef periph_clk_init_struct;

        __HAL_RCC_DMA2D_CLK_ENABLE();
        __HAL_RCC_LTDC_CLK_ENABLE();

        ltdcHandle.Init.HorizontalSync = config.horizontalSync - 1;
        ltdcHandle.Init.VerticalSync = config.verticalSync - 1;
        ltdcHandle.Init.AccumulatedHBP = config.horizontalSync + config.horizontalBackPorch - 1;
        ltdcHandle.Init.AccumulatedVBP = config.verticalSync + config.verticalBackPorch - 1;
        ltdcHandle.Init.AccumulatedActiveH = displaySize.deltaY + config.verticalSync + config.verticalBackPorch - 1;
        ltdcHandle.Init.AccumulatedActiveW = displaySize.deltaX + config.horizontalSync + config.horizontalBackPorch - 1;
        ltdcHandle.Init.TotalWidth = displaySize.deltaX + config.horizontalSync + config.horizontalBackPorch + config.horizontalFrontPorch - 1;
        ltdcHandle.Init.TotalHeigh = displaySize.deltaY + config.verticalSync + config.verticalBackPorch + config.horizontalFrontPorch - 1;

        // Configure PLLSAI prescalers for LCD
        // PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
        // PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz
        // PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/2 = 96 Mhz
        // LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 96/4 = 24 Mhz
        periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
        periph_clk_init_struct.PLLSAI.PLLSAIR = config.frequencyDivisor;
        periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
        HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);

        ltdcHandle.LayerCfg->ImageWidth = displaySize.deltaX;
        ltdcHandle.LayerCfg->ImageHeight = displaySize.deltaY;

        ltdcHandle.Init.Backcolor.Blue = 0;
        ltdcHandle.Init.Backcolor.Green = 0;
        ltdcHandle.Init.Backcolor.Red = 0;
        ltdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
        ltdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
        ltdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
        ltdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
        ltdcHandle.Instance = LTDC;
        HAL_LTDC_Init(&ltdcHandle);
    }

    void LcdStm::InitLayers(const Config& config)
    {
        LTDC_LayerCfgTypeDef layer{};

        layer.WindowX0 = 0;
        layer.WindowX1 = displaySize.deltaX;
        layer.WindowY0 = 0;
        layer.WindowY1 = displaySize.deltaY;
        layer.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
        layer.FBStartAdress = reinterpret_cast<uint32_t>(layers[0].buffer.begin());
        layer.Alpha = 0;
        layer.Alpha0 = 0;
        layer.Backcolor.Blue = 0;
        layer.Backcolor.Green = 0;
        layer.Backcolor.Red = 0;
        layer.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
        layer.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
        layer.ImageWidth = displaySize.deltaX;
        layer.ImageHeight = displaySize.deltaY;

        HAL_LTDC_ConfigLayer(&ltdcHandle, &layer, 0); // Initialize layer 1

        layer.FBStartAdress = reinterpret_cast<uint32_t>(layers[1].buffer.begin());
        HAL_LTDC_ConfigLayer(&ltdcHandle, &layer, 1); // Initialize layer 2
    }

    void LcdStm::OnReload()
    {
        LTDC->IER &= ~LTDC_IER_RRIE;
        LTDC->ICR |= LTDC_ICR_CRRIF;

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    LcdStmDoubleBuffer::LcdStmDoubleBuffer(hal::MultiGpioPinStm& lcdPins, hal::GpioPin& displayEnable, hal::GpioPin& backlightEnable, infra::ByteRange buffer0, infra::ByteRange buffer1, const Config& config)
        : LcdStm(lcdPins, displayEnable, backlightEnable, buffer0, buffer1, config)
        , bitmap0(buffer0, DisplaySize(), config.pixelFormat)
        , bitmap1(buffer1, DisplaySize(), config.pixelFormat)
    {}

    void LcdStmDoubleBuffer::SwapLayers(const infra::Function<void()>& onDone)
    {
        currentLayer = &currentLayer->other;
        LcdStm::SwapLayers(onDone);
    }

    infra::Bitmap& LcdStmDoubleBuffer::DrawingBitmap()
    {
        return currentLayer->bitmap;
    }

    const infra::Bitmap& LcdStmDoubleBuffer::ViewingBitmap() const
    {
        return currentLayer->other.bitmap;
    }

    void LcdStmMultiBuffer::SetBitmap(const infra::Bitmap& bitmap, const infra::Function<void()>& onDone)
    {
        SwapLayers(bitmap.buffer, onDone);
    }
}
