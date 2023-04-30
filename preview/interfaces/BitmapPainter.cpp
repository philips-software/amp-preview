#include "preview/interfaces/BitmapPainter.hpp"

namespace hal
{
    void BitmapPainterCanonical::DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        if (boundingBox.Contains(position))
        {
            WaitUntilDrawingFinished();
            DrawPixel(bitmap, position, infra::ConvertRgb888To(colour, bitmap.pixelFormat));
        }
    }

    void BitmapPainterCanonical::DrawLine(infra::Bitmap& bitmap, infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.y == to.y)
            DrawHorizontalLine(bitmap, infra::Point(std::min(from.x, to.x), from.y), std::abs(from.x - to.x) + 1, colour, boundingBox);
        else if (from.x == to.x)
            DrawVerticalLine(bitmap, infra::Point(from.x, std::min(from.y, to.y)), std::abs(from.y - to.y) + 1, colour, boundingBox);
        else
        {
            infra::Vector increment1(to.x >= from.x ? 1 : -1, to.y >= from.y ? 1 : -1);
            infra::Vector increment2(increment1);

            infra::Vector delta(std::abs(to.x - from.x) + 1, std::abs(to.y - from.y) + 1);

            int16_t denominator = 0;
            int16_t numerator = 0;
            int16_t numeratorIncrement = 0;
            int16_t numberOfPixels = 0;

            if (delta.deltaX >= delta.deltaY)
            {
                increment1.deltaX = 0;
                increment2.deltaY = 0;
                denominator = delta.deltaX;
                numerator = delta.deltaX / 2;
                numeratorIncrement = delta.deltaY;
                numberOfPixels = delta.deltaX;
            }
            else
            {
                increment2.deltaX = 0;
                increment1.deltaY = 0;
                denominator = delta.deltaY;
                numerator = delta.deltaY / 2;
                numeratorIncrement = delta.deltaX;
                numberOfPixels = delta.deltaY;
            }

            for (int16_t currentPixel = 0; currentPixel != numberOfPixels; ++currentPixel)
            {
                DrawPixel(bitmap, from, colour, boundingBox);
                numerator += numeratorIncrement;

                if (numerator >= denominator)
                {
                    numerator -= denominator;
                    from += increment1;
                }

                from += increment2;
            }
        }
    }

    void BitmapPainterCanonical::DrawRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        infra::Region rectangle(position & boundingBox);

        if (rectangle.Left() == position.Left())
            DrawVerticalLine(bitmap, rectangle.TopLeft(), rectangle.Height(), colour);
        if (rectangle.Right() == position.Right())
            DrawVerticalLine(bitmap, rectangle.TopRight() - infra::Vector(1, 0), rectangle.Height(), colour);
        if (rectangle.Top() == position.Top())
            DrawHorizontalLine(bitmap, rectangle.TopLeft(), rectangle.Width(), colour);
        if (rectangle.Bottom() == position.Bottom())
            DrawHorizontalLine(bitmap, rectangle.BottomLeft() - infra::Vector(0, 1), rectangle.Width(), colour);
    }

    void BitmapPainterCanonical::DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        infra::Region destination = position & boundingBox;

        if (!destination.Empty())
        {
            WaitUntilDrawingFinished();

            for (auto y = destination.Top(); y != destination.Bottom(); ++y)
                DrawHorizontalLine(bitmap, infra::Point(destination.Left(), y), destination.Width(), colour);
        }
    }

    void BitmapPainterCanonical::DrawRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        assert(position.Width() >= 2 * r);
        assert(position.Height() >= 2 * r);

        infra::Point topLeftCentre(position.TopLeft() + infra::Vector(r, r));
        infra::Point topRightCentre(position.TopRight() + infra::Vector(-r, r));
        infra::Point bottomLeftCentre(position.BottomLeft() + infra::Vector(r, -r));
        infra::Point bottomRightCentre(position.BottomRight() + infra::Vector(-r, -r));

        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        while (x < y)
        {
            if (f >= 0)
            {
                --y;
                ddF_y += 2;
                f += ddF_y;
            }

            ++x;
            ddF_x += 2;
            f += ddF_x;

            DrawPixel(bitmap, infra::Point(bottomRightCentre.x + x, bottomRightCentre.y + y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(bottomLeftCentre.x - x, bottomLeftCentre.y + y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(topRightCentre.x + x, topRightCentre.y - y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(topLeftCentre.x - x, topLeftCentre.y - y), colour, boundingBox);

            DrawPixel(bitmap, infra::Point(bottomRightCentre.x + y, bottomRightCentre.y + x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(bottomLeftCentre.x - y, bottomLeftCentre.y + x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(topRightCentre.x + y, topRightCentre.y - x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(topLeftCentre.x - y, topLeftCentre.y - x), colour, boundingBox);
        }

        DrawVerticalLine(bitmap, infra::Point(topRightCentre.x + r, topRightCentre.y), bottomRightCentre.y - topRightCentre.y + 1, colour, boundingBox);
        DrawVerticalLine(bitmap, infra::Point(topLeftCentre.x - r, topLeftCentre.y), bottomRightCentre.y - topLeftCentre.y + 1, colour, boundingBox);
        DrawHorizontalLine(bitmap, infra::Point(bottomLeftCentre.x, bottomLeftCentre.y + r), bottomRightCentre.x - bottomLeftCentre.x + 1, colour, boundingBox);
        DrawHorizontalLine(bitmap, infra::Point(topLeftCentre.x, topLeftCentre.y - r), topRightCentre.x - topLeftCentre.x + 1, colour, boundingBox);
    }

    void BitmapPainterCanonical::DrawFilledRoundedRectangle(infra::Bitmap& bitmap, infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        assert(position.Width() >= 2 * r);
        assert(position.Height() >= 2 * r);

        infra::Point topLeftCentre(position.TopLeft() + infra::Vector(r, r));
        infra::Point topRightCentre(position.TopRight() + infra::Vector(-r, r));
        infra::Point bottomLeftCentre(position.BottomLeft() + infra::Vector(r, -r));
        infra::Point bottomRightCentre(position.BottomRight() + infra::Vector(-r, -r));

        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        while (x < y)
        {
            if (f >= 0)
            {
                --y;
                ddF_y += 2;
                f += ddF_y;
            }

            ++x;
            ddF_x += 2;
            f += ddF_x;

            DrawHorizontalLine(bitmap, infra::Point(bottomLeftCentre.x - x, bottomLeftCentre.y + y), bottomRightCentre.x - bottomLeftCentre.x + 2 * x + 1, colour, boundingBox);
            DrawHorizontalLine(bitmap, infra::Point(topLeftCentre.x - x, topLeftCentre.y - y), topRightCentre.x - topLeftCentre.x + 2 * x + 1, colour, boundingBox);
            DrawHorizontalLine(bitmap, infra::Point(bottomLeftCentre.x - y, bottomLeftCentre.y + x), bottomRightCentre.x - bottomLeftCentre.x + 2 * y + 1, colour, boundingBox);
            DrawHorizontalLine(bitmap, infra::Point(topLeftCentre.x - y, topLeftCentre.y - x), topRightCentre.x - topLeftCentre.x + 2 * y + 1, colour, boundingBox);
        }

        DrawFilledRectangle(bitmap, infra::Region(infra::Point(topLeftCentre.x - r, topLeftCentre.y), infra::Vector(topRightCentre.x - topLeftCentre.x + 2 * r + 1, bottomLeftCentre.y - topLeftCentre.y + 1)), colour, boundingBox);
    }

    void BitmapPainterCanonical::DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        DrawPixel(bitmap, infra::Point(centre.x, centre.y + r), colour, boundingBox);
        DrawPixel(bitmap, infra::Point(centre.x, centre.y - r), colour, boundingBox);
        DrawPixel(bitmap, infra::Point(centre.x + r, centre.y), colour, boundingBox);
        DrawPixel(bitmap, infra::Point(centre.x - r, centre.y), colour, boundingBox);

        while (x < y)
        {
            if (f >= 0)
            {
                --y;
                ddF_y += 2;
                f += ddF_y;
            }

            ++x;
            ddF_x += 2;
            f += ddF_x;

            DrawPixel(bitmap, infra::Point(centre.x + x, centre.y + y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x - x, centre.y + y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x + x, centre.y - y), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x - x, centre.y - y), colour, boundingBox);

            DrawPixel(bitmap, infra::Point(centre.x + y, centre.y + x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x - y, centre.y + x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x + y, centre.y - x), colour, boundingBox);
            DrawPixel(bitmap, infra::Point(centre.x - y, centre.y - x), colour, boundingBox);
        }
    }

    void BitmapPainterCanonical::DrawCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t fSmall = 1 - rSmall;
        int16_t fLarge = 1 - rLarge;
        int16_t ddF_x = 1;
        int16_t ddF_ySmall = -2 * rSmall;
        int16_t ddF_yLarge = -2 * rLarge;
        int16_t x = 0;
        int16_t ySmall = rSmall;
        int16_t yLarge = rLarge;

        DrawLine(bitmap, infra::Point(centre.x, centre.y + ySmall), infra::Point(centre.x, centre.y + yLarge), colour, boundingBox);
        DrawLine(bitmap, infra::Point(centre.x, centre.y - ySmall), infra::Point(centre.x, centre.y - yLarge), colour, boundingBox);
        DrawLine(bitmap, infra::Point(centre.x + ySmall, centre.y), infra::Point(centre.x + yLarge, centre.y), colour, boundingBox);
        DrawLine(bitmap, infra::Point(centre.x - ySmall, centre.y), infra::Point(centre.x - yLarge, centre.y), colour, boundingBox);

        while (x < yLarge)
        {
            if (fSmall >= 0)
            {
                --ySmall;
                ddF_ySmall += 2;
                fSmall += ddF_ySmall;
            }

            if (fLarge >= 0)
            {
                --yLarge;
                ddF_yLarge += 2;
                fLarge += ddF_yLarge;
            }

            ++x;
            ddF_x += 2;
            fSmall += ddF_x;
            fLarge += ddF_x;

            DrawLine(bitmap, infra::Point(centre.x + x, centre.y + ySmall), infra::Point(centre.x + x, centre.y + yLarge), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x - x, centre.y + ySmall), infra::Point(centre.x - x, centre.y + yLarge), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x + x, centre.y - ySmall), infra::Point(centre.x + x, centre.y - yLarge), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x - x, centre.y - ySmall), infra::Point(centre.x - x, centre.y - yLarge), colour, boundingBox);

            DrawLine(bitmap, infra::Point(centre.x + ySmall, centre.y + x), infra::Point(centre.x + yLarge, centre.y + x), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x - ySmall, centre.y + x), infra::Point(centre.x - yLarge, centre.y + x), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x + ySmall, centre.y - x), infra::Point(centre.x + yLarge, centre.y - x), colour, boundingBox);
            DrawLine(bitmap, infra::Point(centre.x - ySmall, centre.y - x), infra::Point(centre.x - yLarge, centre.y - x), colour, boundingBox);
        }
    }

    void BitmapPainterCanonical::DrawFilledCircle(infra::Bitmap& bitmap, infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        DrawPixel(bitmap, infra::Point(centre.x, centre.y + r), colour, boundingBox);
        DrawPixel(bitmap, infra::Point(centre.x, centre.y - r), colour, boundingBox);
        DrawHorizontalLine(bitmap, infra::Point(centre.x - r, centre.y), 2 * r + 1, colour, boundingBox);

        while (x < y)
        {
            if (f >= 0)
            {
                --y;
                ddF_y += 2;
                f += ddF_y;
            }

            ++x;
            ddF_x += 2;
            f += ddF_x;

            DrawHorizontalLine(bitmap, infra::Point(centre.x - x, centre.y + y), 2 * x + 1, colour, boundingBox);
            DrawHorizontalLine(bitmap, infra::Point(centre.x - x, centre.y - y), 2 * x + 1, colour, boundingBox);

            DrawHorizontalLine(bitmap, infra::Point(centre.x - y, centre.y + x), 2 * y + 1, colour, boundingBox);
            DrawHorizontalLine(bitmap, infra::Point(centre.x - y, centre.y - x), 2 * y + 1, colour, boundingBox);
        }
    }

    void BitmapPainterCanonical::DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        assert(bitmap.pixelFormat == sourceBitmap.pixelFormat);
        auto bitmapDestination = infra::Region(position, sourceBitmap.size);
        auto boundedDestination = bitmapDestination & boundingBox;

        if (!boundedDestination.Empty())
        {
            WaitUntilDrawingFinished();
            for (auto y = boundedDestination.Top() - bitmapDestination.Top(); y != boundedDestination.Bottom() - bitmapDestination.Top(); ++y)
                for (auto x = boundedDestination.Left() - bitmapDestination.Left(); x != boundedDestination.Right() - bitmapDestination.Left(); ++x)
                {
                    auto colour = sourceBitmap.PixelColour(infra::Point(x, y));
                    DrawPixel(bitmap, bitmapDestination.TopLeft() + infra::Vector(x, y), colour);
                }
        }
    }

    void BitmapPainterCanonical::DrawTransparentBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        assert(bitmap.pixelFormat == sourceBitmap.pixelFormat);
        auto bitmapDestination = infra::Region(position, sourceBitmap.size);
        auto boundedDestination = bitmapDestination & boundingBox;

        if (!boundedDestination.Empty())
        {
            WaitUntilDrawingFinished();
            for (auto y = boundedDestination.Top() - bitmapDestination.Top(); y != boundedDestination.Bottom() - bitmapDestination.Top(); ++y)
                for (auto x = boundedDestination.Left() - bitmapDestination.Left(); x != boundedDestination.Right() - bitmapDestination.Left(); ++x)
                {
                    auto colour = sourceBitmap.PixelColour(infra::Point(x, y));
                    if (colour != transparencyColour)
                        DrawPixel(bitmap, bitmapDestination.TopLeft() + infra::Vector(x, y), colour);
                }
        }
    }

    void BitmapPainterCanonical::DrawIcon(infra::Bitmap& bitmap, infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        assert(sourceBitmap.pixelFormat == infra::PixelFormat::blackandwhite);
        auto iconDestination = infra::Region(position, sourceBitmap.size);
        auto boundedDestination = iconDestination & boundingBox;

        if (!boundedDestination.Empty())
        {
            auto convertedColour = infra::ConvertRgb888To(colour, bitmap.pixelFormat);
            WaitUntilDrawingFinished();
            for (auto y = boundedDestination.Top() - iconDestination.Top(); y != boundedDestination.Bottom() - iconDestination.Top(); ++y)
                for (auto x = boundedDestination.Left() - iconDestination.Left(); x != boundedDestination.Right() - iconDestination.Left(); ++x)
                {
                    if (sourceBitmap.BlackAndWhitePixel(infra::Point(x, y)))
                        DrawPixel(bitmap, iconDestination.TopLeft() + infra::Vector(x, y), convertedColour);
                }
        }
    }

    void BitmapPainterCanonical::DrawString(infra::Bitmap& bitmap, infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        for (auto c : string)
            position = DrawCharacter(bitmap, position, c, font, colour, direction, boundingBox);
    }

    void BitmapPainterCanonical::WaitUntilDrawingFinished() const
    {}

    void BitmapPainterCanonical::DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.y >= boundingBox.Top() && from.y < boundingBox.Bottom() && from.x < boundingBox.Right() && from.x + deltaX >= boundingBox.Left())
        {
            int16_t fromX = std::max(from.x, boundingBox.Left());
            deltaX = std::min<int16_t>(from.x + deltaX, boundingBox.Right()) - fromX;

            DrawHorizontalLine(bitmap, infra::Point(fromX, from.y), deltaX, colour);
        }
    }

    void BitmapPainterCanonical::DrawHorizontalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour)
    {
        if (deltaX > 0)
        {
            WaitUntilDrawingFinished();

            for (auto x = 0; x != deltaX; ++x)
            {
                switch (bitmap.pixelFormat)
                {
                    case infra::PixelFormat::rgb565:
                    {
                        auto convertedColour = infra::ConvertRgb888ToRgb565(colour);
                        std::memcpy(bitmap.BufferAddress(from + infra::DeltaX(x)), &convertedColour, 2);
                        break;
                    }
                    case infra::PixelFormat::rgb888:
                        std::memcpy(bitmap.BufferAddress(from + infra::DeltaX(x)), &colour, 3);
                        break;
                    case infra::PixelFormat::blackandwhite:
                        DrawPixel(bitmap, from + infra::DeltaX(x), infra::ConvertRgb888ToBlackAndWhite(colour));
                        break;
                    default:
                        std::abort();
                }
            }
        }
    }

    void BitmapPainterCanonical::DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.x >= boundingBox.Left() && from.x < boundingBox.Right() && from.y < boundingBox.Bottom() && from.y + deltaY >= boundingBox.Top())
        {
            int16_t fromY = std::max(from.y, boundingBox.Top());
            deltaY = std::min<int16_t>(from.y + deltaY, boundingBox.Bottom()) - fromY;

            DrawVerticalLine(bitmap, infra::Point(from.x, fromY), deltaY, colour);
        }
    }

    void BitmapPainterCanonical::DrawVerticalLine(infra::Bitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour)
    {
        if (deltaY > 0)
        {
            WaitUntilDrawingFinished();

            for (auto y = 0; y != deltaY; ++y)
            {
                switch (bitmap.pixelFormat)
                {
                    case infra::PixelFormat::rgb565:
                    {
                        auto convertedColour = infra::ConvertRgb888ToRgb565(colour);
                        std::memcpy(bitmap.BufferAddress(from + infra::DeltaY(y)), &convertedColour, 2);
                        break;
                    }
                    case infra::PixelFormat::rgb888:
                        std::memcpy(bitmap.BufferAddress(from + infra::DeltaY(y)), &colour, 3);
                        break;
                    case infra::PixelFormat::blackandwhite:
                        DrawPixel(bitmap, from + infra::DeltaY(y), infra::ConvertRgb888ToBlackAndWhite(colour));
                        break;
                    default:
                        std::abort();
                }
            }
        }
    }

    infra::Point BitmapPainterCanonical::DrawCharacter(infra::Bitmap& bitmap, infra::Point position, char c, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        if (c < font.begin || c >= font.end)
            return position;

        const infra::Glyph& glyph = font.glyphs[c - font.begin];

        auto bitmapRegion = infra::Region(infra::Point(), bitmap.size);
        auto backRotatedBitmapRegion = infra::RotatedRegionInRegion(bitmapRegion, -direction, bitmapRegion);
        infra::Point backRotatedPosition = infra::RotatedPointInRegion(position, -direction, bitmapRegion);
        infra::Region unrotatedGlyphRegion(backRotatedPosition + glyph.cursorOffset, glyph.size);
        auto glyphRegion = infra::RotatedRegionInRegion(unrotatedGlyphRegion, direction, backRotatedBitmapRegion);

        if (!(boundingBox & glyphRegion).Empty())
        {
            uint32_t bitIndex = 0;
            infra::ConstByteRange buffer = glyph.buffer;
            uint32_t bitmapColour = infra::ConvertRgb888To(colour, bitmap.pixelFormat);

            WaitUntilDrawingFinished();

            for (uint8_t y = 0; y != unrotatedGlyphRegion.Height(); ++y)
                for (uint8_t x = 0; x != unrotatedGlyphRegion.Width(); ++x, ++bitIndex)
                {
                    auto pixelPosition = infra::RotatedPointInRegion(unrotatedGlyphRegion.TopLeft() + infra::Vector(x, y), direction, backRotatedBitmapRegion);
                    if (boundingBox.Contains(pixelPosition) && (buffer[bitIndex >> 3] & (1 << (7 - (bitIndex % 8)))) != 0)
                        DrawPixel(bitmap, pixelPosition, bitmapColour);
                }
        }

        return position + infra::Rotated(infra::Vector(glyph.advanceCursor, 0), -direction);
    }

    void BitmapPainterCanonical::DrawPixel(infra::Bitmap& bitmap, infra::Point position, uint32_t colour)
    {
        if (bitmap.pixelFormat == infra::PixelFormat::blackandwhite)
            bitmap.SetBlackAndWhitePixel(position, colour != 0);
        else
        {
            switch (infra::PixelSize(bitmap.pixelFormat))
            {
                case 1:
                    *reinterpret_cast<volatile uint8_t*>(bitmap.BufferAddress(position)) = static_cast<uint8_t>(colour);
                    break;
                case 2:
                    *reinterpret_cast<volatile uint16_t*>(bitmap.BufferAddress(position)) = static_cast<uint16_t>(colour);
                    break;
                case 3:
                    *reinterpret_cast<volatile uint16_t*>(bitmap.BufferAddress(position)) = static_cast<uint16_t>(colour);
                    *(reinterpret_cast<volatile uint8_t*>(bitmap.BufferAddress(position)) + 2) = static_cast<uint8_t>(colour >> 16);
                    break;
                case 4:
                    *reinterpret_cast<volatile uint32_t*>(bitmap.BufferAddress(position)) = static_cast<uint32_t>(colour);
                    break;
                default:
                    std::abort();
            }
        }
    }
}
