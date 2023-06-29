#ifndef PREVIEW_BUFFERED_DISPLAY_SSD1306_HPP
#define PREVIEW_BUFFERED_DISPLAY_SSD1306_HPP

#include "hal/interfaces/I2c.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/Optional.hpp"
#include "preview/interfaces/BufferedDisplay.hpp"
#include "preview/interfaces/ViewPainter.hpp"

namespace services
{
    class BufferedDisplaySsd1306
        : public hal::BufferedDisplay
    {
    public:
        explicit BufferedDisplaySsd1306(hal::I2cMaster& i2c, hal::I2cAddress address = hal::I2cAddress(0x3c));

    public:
        // Implementation of BufferedDisplay
        infra::Region DisplayRegion() const override;
        infra::PixelFormat PixelFormat() const override;

        // Additional restrictions: bitmap width and position.x must be a multiple of 8, position.y must be a multiple of 16
        void DrawBitmap(const infra::Bitmap& bitmap, infra::Point position, infra::Function<void()> onDone) override;

    private:
        void DoDrawBitmap();

    private:
        hal::I2cMaster& i2c;
        hal::I2cAddress address;
        bool initializing = true;

        infra::Optional<infra::Bitmap> bitmap;
        infra::Point position;
        infra::AutoResetFunction<void()> onDone;

        std::array<uint8_t, 8> resetPosition = { {
            0, 0x21, 0, 0, // Set Column Address
            0, 0x22, 0, 0, // Set Page Address
        } };
    };

    class ViewPainterAlignedForSsd1306
        : public ViewPainter
    {
    public:
        explicit ViewPainterAlignedForSsd1306(ViewPainter& painter);

        void Paint(services::View& view, infra::Region region, infra::Function<void()> onDone) override;

    private:
        ViewPainter& painter;
    };
}

#endif
