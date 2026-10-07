#include "preview/interfaces/Hub75Bitmap.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <bitset>

namespace infra
{
    Hub75Bitmap::Hub75Bitmap(infra::ByteRange buffer, infra::Vector size, infra::Vector panelSize)
        : infra::Bitmap(size)
        , panelSize(panelSize)
        , buffer(buffer)
        , blockSize(size.deltaX / 32)
    {
        assert(size.deltaY % 32 == 0);
        assert(buffer.size() == BufferSize(size.deltaX, size.deltaY));

        Clear();
    }

    void Hub75Bitmap::Clear()
    {
        for (auto i = 0; i != buffer.size() / 2; ++i)
        {
            buffer[i * 2 + 0] = 0x00;
            buffer[i * 2 + 1] = 0x40;
        }
    }

    infra::Colour Hub75Bitmap::PixelColour(infra::Point position) const
    {
        auto bitPosition{ CalculatePosition(position) };
        auto bitColourPattern = (buffer[bitPosition.bufferPosition] & ~(7 << bitPosition.patternShift)) >> bitPosition.patternShift;

        return CreateColour((bitColourPattern & 1) ? 255 : 0, (bitColourPattern & 2) ? 255 : 0, (bitColourPattern & 4) ? 255 : 0);
    }

    void Hub75Bitmap::DrawPixel(infra::Point position, infra::Colour colour)
    {
        auto red = RedFromColour(colour) != 0;
        auto green = GreenFromColour(colour) != 0;
        auto blue = BlueFromColour(colour) != 0;

        uint8_t bitColourPattern = (red ? 1 : 0) | (green ? 2 : 0) | (blue ? 4 : 0);
        auto bitPosition{ CalculatePosition(position) };

        buffer[bitPosition.bufferPosition] = (buffer[bitPosition.bufferPosition] & ~(7 << bitPosition.patternShift)) | (bitColourPattern << bitPosition.patternShift);
        buffer[bitPosition.bufferPosition + 1] = (buffer[bitPosition.bufferPosition + 1] & ~(7 << bitPosition.patternShift)) | (bitColourPattern << bitPosition.patternShift);
    }

    uint32_t Hub75Bitmap::BufferSize(infra::Vector size)
    {
        return BufferSize(size.deltaX, size.deltaY);
    }

    uint32_t Hub75Bitmap::BufferSize()
    {
        return buffer.size();
    }

    bool Hub75Bitmap::operator==(const Hub75Bitmap& other) const
    {
        return other.size == size && other.buffer == buffer;
    }

    Hub75Bitmap::BitPosition Hub75Bitmap::CalculatePosition(infra::Point position) const
    {
        // Panels are chained in a certain way so that wires are kept short. Since the panels that I have have their input on the
        // right and the output on the left, the panels are ordered right-to-left for the even rows (starting from 0).
        // For the odd rows, in order to wire the top left row straight down, the panels are placed upside down, and
        // therefore running from left to right.
        // This generic chain is therefore created, with panels A, B, C, ...:
        // G -> H -> I
        // ^
        // F <- E <- D  <---- these panels are upside down
        //           ^
        // A -> B -> C

        // The rows of each individual panel are mapped on a virtual display of horizontal size of one panel, and vertical size of (half the) number of panels. Half the
        // number, because since two pixels of each panel are combined, each panel results in half a sized panel in the virtual display

        infra::Point panel(position.x / panelSize.deltaX, position.y / panelSize.deltaY);
        infra::Vector halfPanelSize(panelSize.deltaX, panelSize.deltaY / 2);
        infra::Point positionInPanel(position.x % panelSize.deltaX, position.y % panelSize.deltaY);

        // odd panels are upside down and positioned right-to-left
        if (panel.y % 2 == 1)
        {
            panel.x = size.deltaX / panelSize.deltaX - panel.x - 1;
            positionInPanel = infra::Point(panelSize.deltaX - positionInPanel.x - 1, panelSize.deltaY - positionInPanel.y - 1);
        }

        infra::Point positionInHalfPanel(positionInPanel.x, positionInPanel.y % (halfPanelSize.deltaY));

        panel.y = size.deltaY / panelSize.deltaY - panel.y - 1;

        auto nofPanels = (size.deltaX / panelSize.deltaX) * (size.deltaY / panelSize.deltaY);
        auto displayPanelIndex = panel.x + panel.y * (size.deltaX / panelSize.deltaX);

        infra::Point positionOnDisplay(positionInHalfPanel.x, positionInHalfPanel.y * nofPanels + panel.x + panel.y * (size.deltaY / panelSize.deltaY));

        BitPosition result{ 0, static_cast<uint32_t>((positionOnDisplay.y * halfPanelSize.deltaX + positionOnDisplay.x) * 2) };

        if (positionInPanel.y >= panelSize.deltaY / 2)
            result.patternShift = 3;

        return result;
    }
}
