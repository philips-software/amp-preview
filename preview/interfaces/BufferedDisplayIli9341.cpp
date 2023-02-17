#include "preview/interfaces/BufferedDisplayIli9341.hpp"
#include "infra/event/EventDispatcher.hpp"

#define ILI9341_NOP 0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID 0x04
#define ILI9341_RDDST 0x09

#define ILI9341_SLPIN 0x10
#define ILI9341_SLPOUT 0x11
#define ILI9341_PTLON 0x12
#define ILI9341_NORON 0x13

#define ILI9341_RDMODE 0x0A
#define ILI9341_RDMADCTL 0x0B
#define ILI9341_RDPIXFMT 0x0C
#define ILI9341_RDIMGFMT 0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF 0x20
#define ILI9341_INVON 0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON 0x29

#define ILI9341_CASET 0x2A
#define ILI9341_PASET 0x2B
#define ILI9341_RAMWR 0x2C
#define ILI9341_RAMRD 0x2E

#define ILI9341_PTLAR 0x30
#define ILI9341_MADCTL 0x36
#define ILI9341_PIXFMT 0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR 0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1 0xC0
#define ILI9341_PWCTR2 0xC1
#define ILI9341_PWCTR3 0xC2
#define ILI9341_PWCTR4 0xC3
#define ILI9341_PWCTR5 0xC4
#define ILI9341_VMCTR1 0xC5
#define ILI9341_VMCTR2 0xC7

#define ILI9341_RDID1 0xDA
#define ILI9341_RDID2 0xDB
#define ILI9341_RDID3 0xDC
#define ILI9341_RDID4 0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

namespace services
{
    static std::array<uint8_t, 4> displayIdentification;

    BufferedDisplayIli9341::BufferedDisplayIli9341(hal::SpiMaster& spi, hal::GpioPin& dataCommandPin, hal::CommunicationConfigurator& spiWidth16Configurator)
        : spi(spi)
        , dataCommandPin(dataCommandPin)
        , spiWidth16Configurator(spiWidth16Configurator)
    {
        InitDisplay();
    }

    infra::Region BufferedDisplayIli9341::DisplayRegion() const
    {
        return infra::Region(infra::Point(), infra::Vector(320, 240));
    }

    infra::PixelFormat BufferedDisplayIli9341::PixelFormat() const
    {
        return infra::PixelFormat::rgb565;
    }

    void BufferedDisplayIli9341::DrawBitmap(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone)
    {
        this->onDone = onDone;

        x0 = position.x;
        x1 = (position + bitmap.size).x - 1;
        y0 = position.y;
        y1 = (position + bitmap.size).y - 1;

        x0 = (x0 << 8) | (x0 >> 8);
        x1 = (x1 << 8) | (x1 >> 8);
        y0 = (y0 << 8) | (y0 >> 8);
        y1 = (y1 << 8) | (y1 >> 8);

        bitmapBuffer = bitmap.buffer;

        if (sequencer.Finished())
            SendBitmap();
    }

    void BufferedDisplayIli9341::InitDisplay()
    {
        static const std::array<uint8_t, 1> initCommand0{ 0xEF };
        static const std::array<uint8_t, 3> initData0{ 0x03, 0x80, 0x02 };
        static const std::array<uint8_t, 1> initCommand1{ 0xCF };
        static const std::array<uint8_t, 3> initData1{ 0x00, 0xC1, 0X30 };
        static const std::array<uint8_t, 1> initCommand2{ 0xED };
        static const std::array<uint8_t, 4> initData2{ 0x64, 0x03, 0x12, 0x81 };
        static const std::array<uint8_t, 1> initCommand3{ 0xe8 };
        static const std::array<uint8_t, 3> initData3{ 0x85, 0x00, 0x78 };
        static const std::array<uint8_t, 1> initCommand4{ 0xcb };
        static const std::array<uint8_t, 5> initData4{ 0x39, 0x2c, 0x00, 0x34, 0x02 };
        static const std::array<uint8_t, 1> initCommand5{ 0xf7 };
        static const std::array<uint8_t, 1> initData5{ 0x20 };
        static const std::array<uint8_t, 1> initCommand6{ 0xea };
        static const std::array<uint8_t, 2> initData6{ 0x00, 0x00 };
        static const std::array<uint8_t, 1> initCommand7{ ILI9341_PWCTR1 };
        static const std::array<uint8_t, 1> initData7{ 0x23 };
        static const std::array<uint8_t, 1> initCommand8{ ILI9341_PWCTR2 };
        static const std::array<uint8_t, 1> initData8{ 0x10 };
        static const std::array<uint8_t, 1> initCommand9{ ILI9341_VMCTR1 };
        static const std::array<uint8_t, 2> initData9{ 0x3e, 0x28 };
        static const std::array<uint8_t, 1> initCommand10{ ILI9341_VMCTR2 };
        static const std::array<uint8_t, 1> initData10{ 0x86 };
        static const std::array<uint8_t, 1> initCommand11{ ILI9341_MADCTL };
        static const std::array<uint8_t, 1> initData11{ 0x28 }; // Landscape
        static const std::array<uint8_t, 1> initCommand12{ ILI9341_PIXFMT };
        static const std::array<uint8_t, 1> initData12{ 0x55 };
        static const std::array<uint8_t, 1> initCommand13{ ILI9341_FRMCTR1 };
        static const std::array<uint8_t, 2> initData13{ 0x00, 0x18 };
        static const std::array<uint8_t, 1> initCommand14{ ILI9341_DFUNCTR };
        static const std::array<uint8_t, 3> initData14{ 0x08, 0x82, 0x27 };
        static const std::array<uint8_t, 1> initCommand15{ 0xF2 };
        static const std::array<uint8_t, 1> initData15{ 0x00 };
        static const std::array<uint8_t, 1> initCommand16{ ILI9341_GAMMASET };
        static const std::array<uint8_t, 1> initData16{ 0x01 };
        static const std::array<uint8_t, 1> initCommand17{ ILI9341_GMCTRP1 };
        static const std::array<uint8_t, 15> initData17{ 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        static const std::array<uint8_t, 1> initCommand18{ ILI9341_GMCTRN1 };
        static const std::array<uint8_t, 15> initData18{ 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        static const std::array<uint8_t, 1> initCommandEndian{ 0xf6 };
        static const std::array<uint8_t, 3> initDataEndian{ 0x01, 0x00, 0x20 };
        static const std::array<uint8_t, 1> initCommand19{ ILI9341_SLPOUT };
        static const std::array<uint8_t, 1> initCommand20{ ILI9341_DISPON };

        sequencer.Load([this]()
            {
            sequencer.Step([this]() { delayTimer.Start(std::chrono::milliseconds(150), [this]() { sequencer.Continue(); }); });
            sequencer.Step([this]() { SendCommand(initCommand0); });
            sequencer.Step([this]() { SendData(initData0); });
            sequencer.Step([this]() { SendCommand(initCommand1); });
            sequencer.Step([this]() { SendData(initData1); });
            sequencer.Step([this]() { SendCommand(initCommand2); });
            sequencer.Step([this]() { SendData(initData2); });
            sequencer.Step([this]() { SendCommand(initCommand3); });
            sequencer.Step([this]() { SendData(initData3); });
            sequencer.Step([this]() { SendCommand(initCommand4); });
            sequencer.Step([this]() { SendData(initData4); });
            sequencer.Step([this]() { SendCommand(initCommand5); });
            sequencer.Step([this]() { SendData(initData5); });
            sequencer.Step([this]() { SendCommand(initCommand6); });
            sequencer.Step([this]() { SendData(initData6); });
            sequencer.Step([this]() { SendCommand(initCommand7); });
            sequencer.Step([this]() { SendData(initData7); });
            sequencer.Step([this]() { SendCommand(initCommand8); });
            sequencer.Step([this]() { SendData(initData8); });
            sequencer.Step([this]() { SendCommand(initCommand9); });
            sequencer.Step([this]() { SendData(initData9); });
            sequencer.Step([this]() { SendCommand(initCommand10); });
            sequencer.Step([this]() { SendData(initData10); });
            sequencer.Step([this]() { SendCommand(initCommand11); });
            sequencer.Step([this]() { SendData(initData11); });
            sequencer.Step([this]() { SendCommand(initCommand12); });
            sequencer.Step([this]() { SendData(initData12); });
            sequencer.Step([this]() { SendCommand(initCommand13); });
            sequencer.Step([this]() { SendData(initData13); });
            sequencer.Step([this]() { SendCommand(initCommand14); });
            sequencer.Step([this]() { SendData(initData14); });
            sequencer.Step([this]() { SendCommand(initCommand15); });
            sequencer.Step([this]() { SendData(initData15); });
            sequencer.Step([this]() { SendCommand(initCommand16); });
            sequencer.Step([this]() { SendData(initData16); });
            sequencer.Step([this]() { SendCommand(initCommand17); });
            sequencer.Step([this]() { SendData(initData17); });
            sequencer.Step([this]() { SendCommand(initCommand18); });
            sequencer.Step([this]() { SendData(initData18); });
            sequencer.Step([this]() { SendCommand(initCommandEndian); });
            sequencer.Step([this]() { SendData(initDataEndian); });
            sequencer.Step([this]() { SendCommand(initCommand19); });
            sequencer.Step([this]() { delayTimer.Start(std::chrono::milliseconds(120), [this]() { sequencer.Continue(); }); });
            sequencer.Step([this]() { SendCommand(initCommand20); });
            sequencer.Execute([this]() { if (onDone) infra::EventDispatcher::Instance().Schedule([this]() { SendBitmap(); }); }); });
    }

    void BufferedDisplayIli9341::SendBitmap()
    {
        static const uint8_t commandColumnAddressSet = ILI9341_CASET;
        static const uint8_t commandPageAddressSet = ILI9341_PASET;
        static const uint8_t commandRamWrite = ILI9341_RAMWR;

        sequencer.Load([this]()
            {
            sequencer.Step([this]() { SendCommand(infra::MakeByteRange(commandColumnAddressSet)); });
            sequencer.Step([this]() { SendData(infra::MakeByteRange(x0)); });
            sequencer.Step([this]() { SendData(infra::MakeByteRange(x1)); });
            sequencer.Step([this]() { SendCommand(infra::MakeByteRange(commandPageAddressSet)); });
            sequencer.Step([this]() { SendData(infra::MakeByteRange(y0)); });
            sequencer.Step([this]() { SendData(infra::MakeByteRange(y1)); });
            sequencer.Step([this]() { SendCommand(infra::MakeByteRange(commandRamWrite)); });
            sequencer.Execute([this]() { spi.SetCommunicationConfigurator(spiWidth16Configurator); });
            sequencer.Step([this]() { SendData(bitmapBuffer); });
            sequencer.Execute([this]() { spi.ResetCommunicationConfigurator(); });
            sequencer.Execute([this]() { infra::EventDispatcher::Instance().Schedule(onDone); onDone = nullptr; }); });
    }

    void BufferedDisplayIli9341::SendCommand(infra::ConstByteRange command)
    {
        dataCommandPin.Set(false);
        spi.SendData(command, hal::SpiAction::stop, [this]()
            { sequencer.Continue(); });
    }

    void BufferedDisplayIli9341::SendData(infra::ConstByteRange data)
    {
        dataCommandPin.Set(true);
        spi.SendData(data, hal::SpiAction::stop, [this]()
            { sequencer.Continue(); });
    }
}
