#include "preview/interfaces/BufferedDisplaySsd1306.hpp"
#include "infra/util/BoundedVector.hpp"

namespace services
{
    BufferedDisplaySsd1306::BufferedDisplaySsd1306(hal::I2cMaster& i2c, hal::I2cAddress address)
        : i2c(i2c)
        , address(address)
    {
        static const std::array<uint8_t, 37> initCommands{ {
            0, 0xae,       // Set display on/off (Display OFF (sleep mode))
            0, 0xa6,       // Set normal/inverse display (Normal display)
            0, 0xa8, 0x1f, // Set multiplex ratio (N = 32)
            0, 0xd3, 00,   // Set display offset (Vertical shift = 0)
            0, 0x40,       // Set display RAM display start line register (0)
            0, 0xa1,       // Set segment re-map (Column address 127 is mapped to SEG0)
            0, 0xc0,       // Set COM Output Scan Direction (Scan from COM0 to COM[N-1])
            0, 0xda, 0x02, // Set COM pins hardware configuration (Sequential COM pin configuration, disable COM left/right remap)
            0, 0x81, 0x3f, // Set contrast control register (63)
            0, 0xa4,       // Entire display on (Output follows RAM content)
            0, 0xd5, 0x80, // Set display clock divide ratio/oscillator frequency (Divide ratio = 0x0, oscillator frequency = 0x8)
            0, 0x8d, 0x14, // Charge pump setting (Enable charge pump during display on)
            0, 0x2e,       // Deactivate Scroll
            0, 0x20, 0x01, // Set addressing mode (Vertical)
            0, 0xaf        // Set display on/off (Display ON in normal mode)
        } };

        i2c.SendData(address, initCommands, hal::Action::stop, [this](hal::Result result, uint32_t numberOfBytesSent)
            {
            assert(result == hal::Result::complete);
            initializing = false;
            if (onDone != nullptr)
                DoDrawBitmap(); });
    }

    infra::Region BufferedDisplaySsd1306::DisplayRegion() const
    {
        return infra::Region(infra::Point(), infra::Vector(32, 128));
    }

    infra::PixelFormat BufferedDisplaySsd1306::PixelFormat() const
    {
        return infra::PixelFormat::blackandwhite;
    }

    void BufferedDisplaySsd1306::DrawBitmap(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone)
    {
        assert(bitmap.pixelFormat == PixelFormat());
        assert(position.x % 8 == 0);
        assert(position.y % 16 == 0); // Undocumented: Apparently the y position is aligned on multiples of 16
        assert(bitmap.size.deltaX % 8 == 0);
        assert(DisplayRegion().ContainsOrOnBoundary(position + bitmap.size));

        this->bitmap = bitmap;
        this->position = position;
        this->onDone = onDone;

        if (!initializing)
            DoDrawBitmap();
    }

    void BufferedDisplaySsd1306::DoDrawBitmap()
    {
        resetPosition[2] = static_cast<uint8_t>(position.y);
        resetPosition[3] = position.y + bitmap->size.deltaY - 1;
        resetPosition[6] = position.x / 8;
        resetPosition[7] = (position.x + bitmap->size.deltaX) / 8 - 1;

        i2c.SendData(address, infra::MakeByteRange(resetPosition), hal::Action::repeatedStart, [this](hal::Result result, uint32_t numberOfBytesSent)
            {
            static const uint8_t data = 0x40;
            i2c.SendData(address, infra::MakeByteRange(data), hal::Action::continueSession, [this](hal::Result result, uint32_t numberOfBytesSent)
            {
                i2c.SendData(address, bitmap->buffer, hal::Action::stop, [this](hal::Result result, uint32_t numberOfBytesSent)
                {
                    this->onDone();
                });
            }); });
    }

    ViewPainterAlignedForSsd1306::ViewPainterAlignedForSsd1306(ViewPainter& painter)
        : painter(painter)
    {}

    void ViewPainterAlignedForSsd1306::Paint(services::View& view, infra::Region region, infra::Function<void()> onDone)
    {
        auto alignedRegion = infra::AlignedRegion(region, 8, 16);
        alignedRegion.Resize(infra::DeltaX(alignedRegion.Width() + region.Right() - alignedRegion.Right()));
        painter.Paint(view, alignedRegion, onDone);
    }
}
