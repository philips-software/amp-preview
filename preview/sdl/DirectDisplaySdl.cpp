#include "preview/sdl/DirectDisplaySdl.hpp"
#include "preview/interfaces/Colour.hpp"
#include <SDL.h>

namespace
{
    SDL_Rect Convert(infra::Region region)
    {
        return SDL_Rect{ region.TopLeft().x, region.TopLeft().y, region.Width(), region.Height() };
    }
}

namespace hal
{
    DirectDisplaySdl::DirectDisplaySdl(infra::Vector size)
        : size(size)
    {
        SDL_Window* rawWindow = nullptr;
        SDL_Renderer* rawRenderer = nullptr;

        SDL_Init(SDL_INIT_VIDEO);

        SDL_CreateWindowAndRenderer(size.deltaX, size.deltaY, SDL_WINDOW_SHOWN, &rawWindow, &rawRenderer);

        window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(rawWindow, SDL_DestroyWindow);
        renderer = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>(rawRenderer, SDL_DestroyRenderer);

        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer.get());
    }

    DirectDisplaySdl::~DirectDisplaySdl()
    {
        SDL_Quit();
    }

    infra::Vector DirectDisplaySdl::Size() const
    {
        return size;
    }

    void DirectDisplaySdl::DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        if (boundingBox.Contains(position))
            DrawPixel(position, colour);
    }

    void DirectDisplaySdl::DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.y == to.y)
            DrawHorizontalLine(infra::Point(std::min(from.x, to.x), from.y), std::abs(from.x - to.x), colour, boundingBox);
        else if (from.x == to.x)
            DrawVerticalLine(infra::Point(from.x, std::min(from.y, to.y)), std::abs(from.y - to.y), colour, boundingBox);
        else
        {
            infra::Vector increment1(to.x >= from.x ? 1 : -1, to.y >= from.y ? 1 : -1);
            infra::Vector increment2(increment1);

            infra::Vector delta(std::abs(to.x - from.x), std::abs(to.y - from.y));

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

            for (int16_t currentPixel = 0; currentPixel <= numberOfPixels; ++currentPixel)
            {
                DrawPixel(from, colour, boundingBox);
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

    void DirectDisplaySdl::DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        infra::Region rectangle(position & boundingBox);

        if (rectangle.Left() == position.Left())
            DrawVerticalLine(rectangle.TopLeft(), rectangle.Height(), colour);
        if (rectangle.Right() == position.Right())
            DrawVerticalLine(rectangle.TopRight() - infra::Vector(1, 0), rectangle.Height(), colour);
        if (rectangle.Top() == position.Top())
            DrawHorizontalLine(rectangle.TopLeft(), rectangle.Width(), colour);
        if (rectangle.Bottom() == position.Bottom())
            DrawHorizontalLine(rectangle.BottomLeft() - infra::Vector(0, 1), rectangle.Width(), colour);
    }

    void DirectDisplaySdl::DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        infra::Region destination = position & boundingBox;

        if (!destination.Empty())
        {
            SDL_Rect rectangle = Convert(destination);

            SetRenderColour(colour);
            SDL_RenderFillRect(renderer.get(), &rectangle);
        }
    }

    void DirectDisplaySdl::DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
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

            DrawPixel(infra::Point(bottomRightCentre.x + x, bottomRightCentre.y + y), colour, boundingBox);
            DrawPixel(infra::Point(bottomLeftCentre.x - x, bottomLeftCentre.y + y), colour, boundingBox);
            DrawPixel(infra::Point(topRightCentre.x + x, topRightCentre.y - y), colour, boundingBox);
            DrawPixel(infra::Point(topLeftCentre.x - x, topLeftCentre.y - y), colour, boundingBox);

            DrawPixel(infra::Point(bottomRightCentre.x + y, bottomRightCentre.y + x), colour, boundingBox);
            DrawPixel(infra::Point(bottomLeftCentre.x - y, bottomLeftCentre.y + x), colour, boundingBox);
            DrawPixel(infra::Point(topRightCentre.x + y, topRightCentre.y - x), colour, boundingBox);
            DrawPixel(infra::Point(topLeftCentre.x - y, topLeftCentre.y - x), colour, boundingBox);
        }

        DrawVerticalLine(infra::Point(topRightCentre.x + r, topRightCentre.y), bottomRightCentre.y - topRightCentre.y + 1, colour, boundingBox);
        DrawVerticalLine(infra::Point(topLeftCentre.x - r, topLeftCentre.y), bottomRightCentre.y - topLeftCentre.y + 1, colour, boundingBox);
        DrawHorizontalLine(infra::Point(bottomLeftCentre.x, bottomLeftCentre.y + r), bottomRightCentre.x - bottomLeftCentre.x + 1, colour, boundingBox);
        DrawHorizontalLine(infra::Point(topLeftCentre.x, topLeftCentre.y - r), topRightCentre.x - topLeftCentre.x + 1, colour, boundingBox);
    }

    void DirectDisplaySdl::DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
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

            DrawHorizontalLine(infra::Point(bottomLeftCentre.x - x, bottomLeftCentre.y + y), bottomRightCentre.x - bottomLeftCentre.x + 2 * x + 1, colour, boundingBox);
            DrawHorizontalLine(infra::Point(topLeftCentre.x - x, topLeftCentre.y - y), topRightCentre.x - topLeftCentre.x + 2 * x + 1, colour, boundingBox);
            DrawHorizontalLine(infra::Point(bottomLeftCentre.x - y, bottomLeftCentre.y + x), bottomRightCentre.x - bottomLeftCentre.x + 2 * y + 1, colour, boundingBox);
            DrawHorizontalLine(infra::Point(topLeftCentre.x - y, topLeftCentre.y - x), topRightCentre.x - topLeftCentre.x + 2 * y + 1, colour, boundingBox);
        }

        DrawFilledRectangle(infra::Region(infra::Point(topLeftCentre.x - r, topLeftCentre.y), infra::Vector(topRightCentre.x - topLeftCentre.x + 2 * r + 1, bottomLeftCentre.y - topLeftCentre.y + 1)), colour, boundingBox);
    }

    void DirectDisplaySdl::DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        DrawPixel(infra::Point(centre.x, centre.y + r), colour, boundingBox);
        DrawPixel(infra::Point(centre.x, centre.y - r), colour, boundingBox);
        DrawPixel(infra::Point(centre.x + r, centre.y), colour, boundingBox);
        DrawPixel(infra::Point(centre.x - r, centre.y), colour, boundingBox);

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

            DrawPixel(infra::Point(centre.x + x, centre.y + y), colour, boundingBox);
            DrawPixel(infra::Point(centre.x - x, centre.y + y), colour, boundingBox);
            DrawPixel(infra::Point(centre.x + x, centre.y - y), colour, boundingBox);
            DrawPixel(infra::Point(centre.x - x, centre.y - y), colour, boundingBox);

            DrawPixel(infra::Point(centre.x + y, centre.y + x), colour, boundingBox);
            DrawPixel(infra::Point(centre.x - y, centre.y + x), colour, boundingBox);
            DrawPixel(infra::Point(centre.x + y, centre.y - x), colour, boundingBox);
            DrawPixel(infra::Point(centre.x - y, centre.y - x), colour, boundingBox);
        }
    }

    void DirectDisplaySdl::DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t fSmall = 1 - rSmall;
        int16_t fLarge = 1 - rLarge;
        int16_t ddF_x = 1;
        int16_t ddF_ySmall = -2 * rSmall;
        int16_t ddF_yLarge = -2 * rLarge;
        int16_t x = 0;
        int16_t ySmall = rSmall;
        int16_t yLarge = rLarge;

        DrawLine(infra::Point(centre.x, centre.y + ySmall), infra::Point(centre.x, centre.y + yLarge), colour, boundingBox);
        DrawLine(infra::Point(centre.x, centre.y - ySmall), infra::Point(centre.x, centre.y - yLarge), colour, boundingBox);
        DrawLine(infra::Point(centre.x + ySmall, centre.y), infra::Point(centre.x + yLarge, centre.y), colour, boundingBox);
        DrawLine(infra::Point(centre.x - ySmall, centre.y), infra::Point(centre.x - yLarge, centre.y), colour, boundingBox);

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

            DrawLine(infra::Point(centre.x + x, centre.y + ySmall), infra::Point(centre.x + x, centre.y + yLarge), colour, boundingBox);
            DrawLine(infra::Point(centre.x - x, centre.y + ySmall), infra::Point(centre.x - x, centre.y + yLarge), colour, boundingBox);
            DrawLine(infra::Point(centre.x + x, centre.y - ySmall), infra::Point(centre.x + x, centre.y - yLarge), colour, boundingBox);
            DrawLine(infra::Point(centre.x - x, centre.y - ySmall), infra::Point(centre.x - x, centre.y - yLarge), colour, boundingBox);

            DrawLine(infra::Point(centre.x + ySmall, centre.y + x), infra::Point(centre.x + yLarge, centre.y + x), colour, boundingBox);
            DrawLine(infra::Point(centre.x - ySmall, centre.y + x), infra::Point(centre.x - yLarge, centre.y + x), colour, boundingBox);
            DrawLine(infra::Point(centre.x + ySmall, centre.y - x), infra::Point(centre.x + yLarge, centre.y - x), colour, boundingBox);
            DrawLine(infra::Point(centre.x - ySmall, centre.y - x), infra::Point(centre.x - yLarge, centre.y - x), colour, boundingBox);
        }
    }

    void DirectDisplaySdl::DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        DrawPixel(infra::Point(centre.x, centre.y + r), colour, boundingBox);
        DrawPixel(infra::Point(centre.x, centre.y - r), colour, boundingBox);
        DrawLine(infra::Point(centre.x - r, centre.y), infra::Point(centre.x + r, centre.y), colour, boundingBox);

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

            DrawLine(infra::Point(centre.x - x, centre.y + y), infra::Point(centre.x + x, centre.y + y), colour, boundingBox);
            DrawLine(infra::Point(centre.x - x, centre.y - y), infra::Point(centre.x + x, centre.y - y), colour, boundingBox);

            DrawLine(infra::Point(centre.x - y, centre.y + x), infra::Point(centre.x + y, centre.y + x), colour, boundingBox);
            DrawLine(infra::Point(centre.x - y, centre.y - x), infra::Point(centre.x + y, centre.y - x), colour, boundingBox);
        }
    }

    void DirectDisplaySdl::DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        infra::Region destination = infra::Region(position, sourceBitmap.size) & boundingBox;

        if (!destination.Empty())
        {
            infra::Vector bitmapShift = destination.TopLeft() - position;

            if (sourceBitmap.pixelFormat == infra::PixelFormat::blackandwhite)
            {
                for (auto y = 0; y != sourceBitmap.size.deltaY; ++y)
                    for (auto x = 0; x != sourceBitmap.size.deltaX; ++x)
                    {
                        auto i = y * sourceBitmap.size.deltaX + x;
                        auto colour = (sourceBitmap.buffer[i / 8] & (1 << (7 - i % 8))) == 0 ? infra::Colour::black : infra::Colour::white;
                        DrawPixel(destination.TopLeft() + infra::Vector(x, y), colour, boundingBox);
                    }
            }
            else
            {
                for (auto y = 0; y != sourceBitmap.size.deltaY; ++y)
                    for (auto x = 0; x != sourceBitmap.size.deltaX; ++x)
                    {
                        auto colour = sourceBitmap.PixelColour(infra::Point(x, y));
                        DrawPixel(destination.TopLeft() + infra::Vector(x, y), infra::ConvertToRgb888(colour, sourceBitmap.pixelFormat));
                    }
            }
        }
    }

    void DirectDisplaySdl::DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        infra::Region destination = infra::Region(position, sourceBitmap.size) & boundingBox;

        if (!destination.Empty())
        {
            infra::Vector bitmapShift = destination.TopLeft() - position;

            if (sourceBitmap.pixelFormat == infra::PixelFormat::blackandwhite)
            {
                for (auto y = 0; y != sourceBitmap.size.deltaY; ++y)
                    for (auto x = 0; x != sourceBitmap.size.deltaX; ++x)
                    {
                        auto i = y * sourceBitmap.size.deltaX + x;
                        auto colour = (sourceBitmap.buffer[i / 8] & (1 << (7 - i % 8))) == 0;
                        if (colour != (transparencyColour != 0))
                            DrawPixel(destination.TopLeft() + infra::Vector(x, y), colour ? infra::Colour::black : infra::Colour::white, boundingBox);
                    }
            }
            else
            {
                for (auto y = 0; y != sourceBitmap.size.deltaY; ++y)
                    for (auto x = 0; x != sourceBitmap.size.deltaX; ++x)
                    {
                        auto colour = sourceBitmap.PixelColour(infra::Point(x, y));
                        if (colour != transparencyColour)
                            DrawPixel(destination.TopLeft() + infra::Vector(x, y), infra::ConvertToRgb888(colour, sourceBitmap.pixelFormat));
                    }
            }
        }
    }

    void DirectDisplaySdl::DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        assert(sourceBitmap.pixelFormat == infra::PixelFormat::blackandwhite);
        infra::Region destination = infra::Region(position, sourceBitmap.size) & boundingBox;

        if (!destination.Empty())
        {
            infra::Vector bitmapShift = destination.TopLeft() - position;

            for (auto y = 0; y != sourceBitmap.size.deltaY; ++y)
                for (auto x = 0; x != sourceBitmap.size.deltaX; ++x)
                {
                    auto i = y * sourceBitmap.size.deltaX + x;
                    if ((sourceBitmap.buffer[i / 8] & (1 << (7 - i % 8))) != 0)
                        DrawPixel(destination.TopLeft() + infra::Vector(x, y), colour, boundingBox);
                }
        }
    }

    void DirectDisplaySdl::DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        for (auto c : string)
            position = DrawCharacter(position, c, font, colour, direction, boundingBox);
    }

    void DirectDisplaySdl::PaintingComplete()
    {
        SDL_RenderPresent(renderer.get());
    }

    void DirectDisplaySdl::SetRenderColour(infra::Colour colour)
    {
        auto intColour = static_cast<std::underlying_type<infra::Colour>::type>(colour);
        SDL_SetRenderDrawColor(renderer.get(), (intColour >> 16) & 0xFF, (intColour >> 8) & 0xFF, intColour & 0xFF, (intColour >> 24) & 0xFF);
    }

    void DirectDisplaySdl::DrawHorizontalLine(infra::Point from, uint16_t deltaX, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.y >= boundingBox.Top() && from.y < boundingBox.Bottom() && from.x < boundingBox.Right() && from.x + deltaX >= boundingBox.Left())
        {
            int16_t fromX = std::max(from.x, boundingBox.Left());
            deltaX = std::min<int16_t>(from.x + deltaX, boundingBox.Right()) - fromX;

            DrawHorizontalLine(infra::Point(fromX, from.y), deltaX, colour);
        }
    }

    void DirectDisplaySdl::DrawHorizontalLine(infra::Point from, uint16_t deltaX, infra::Colour colour)
    {
        if (deltaX > 0)
        {
            SetRenderColour(colour);
            SDL_RenderDrawLine(renderer.get(), from.x, from.y, from.x + deltaX, from.y);
        }
    }

    void DirectDisplaySdl::DrawVerticalLine(infra::Point from, uint16_t deltaY, infra::Colour colour, infra::Region boundingBox)
    {
        if (from.x >= boundingBox.Left() && from.x < boundingBox.Right() && from.y < boundingBox.Bottom() && from.y + deltaY >= boundingBox.Top())
        {
            int16_t fromY = std::max(from.y, boundingBox.Top());
            deltaY = std::min<int16_t>(from.y + deltaY, boundingBox.Bottom()) - fromY;

            DrawVerticalLine(infra::Point(from.x, fromY), deltaY, colour);
        }
    }

    void DirectDisplaySdl::DrawVerticalLine(infra::Point from, uint16_t deltaY, infra::Colour colour)
    {
        if (deltaY > 0)
        {
            SetRenderColour(colour);
            SDL_RenderDrawLine(renderer.get(), from.x, from.y, from.x, from.y + deltaY);
        }
    }

    infra::Point DirectDisplaySdl::DrawCharacter(infra::Point position, char c, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        if (c < font.begin || c >= font.end)
            return position;

        const infra::Glyph& glyph = font.glyphs[c - font.begin];

        auto bitmapRegion = infra::Region(infra::Point(), size);
        auto backRotatedBitmapRegion = infra::RotatedRegionInRegion(bitmapRegion, -direction, bitmapRegion);
        infra::Point backRotatedPosition = infra::RotatedPointInRegion(position, -direction, bitmapRegion);
        infra::Region unrotatedGlyphRegion(backRotatedPosition + glyph.cursorOffset, glyph.size);
        auto glyphRegion = infra::RotatedRegionInRegion(unrotatedGlyphRegion, direction, backRotatedBitmapRegion);

        if (!(boundingBox & glyphRegion).Empty())
        {
            uint32_t bitIndex = 0;
            infra::ConstByteRange buffer = glyph.buffer;

            for (uint8_t y = 0; y != unrotatedGlyphRegion.Height(); ++y)
                for (uint8_t x = 0; x != unrotatedGlyphRegion.Width(); ++x, ++bitIndex)
                {
                    auto pixelPosition = infra::RotatedPointInRegion(unrotatedGlyphRegion.TopLeft() + infra::Vector(x, y), direction, backRotatedBitmapRegion);
                    if (boundingBox.Contains(pixelPosition) && (buffer[bitIndex >> 3] & (1 << (7 - (bitIndex % 8)))) != 0)
                        DrawPixel(pixelPosition, colour);
                }
        }

        return position + infra::Rotated(infra::Vector(glyph.advanceCursor, 0), -direction);
    }

    void DirectDisplaySdl::DrawPixel(infra::Point position, infra::Colour colour)
    {
        SetRenderColour(colour);
        SDL_RenderDrawPoint(renderer.get(), position.x, position.y);
    }
}
