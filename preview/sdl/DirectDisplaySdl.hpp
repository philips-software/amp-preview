#ifndef PREVIEW_DIRECT_DISPLAY_SDL_HPP
#define PREVIEW_DIRECT_DISPLAY_SDL_HPP

#include "preview/interfaces/DirectDisplay.hpp"
#include <functional>
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace hal
{
    class DirectDisplaySdl
        : public hal::DirectDisplay
    {
    public:
        DirectDisplaySdl(infra::Vector size);
        DirectDisplaySdl(const DirectDisplaySdl& other) = delete;
        DirectDisplaySdl& operator=(const DirectDisplaySdl& other) = delete;
        virtual ~DirectDisplaySdl();

    public:
        // From hal::Canvas
        infra::Vector Size() const override final;

        void DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override final;
        void DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override final;
        void DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override final;
        void DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override final;

        // From hal::DirectDisplay
        void PaintingComplete() override final;

    private:
        void SetRenderColour(infra::Colour colour);
        void DrawHorizontalLine(infra::Point from, uint16_t deltaX, infra::Colour colour, infra::Region boundingBox);
        void DrawHorizontalLine(infra::Point from, uint16_t deltaX, infra::Colour colour);
        void DrawVerticalLine(infra::Point from, uint16_t deltaY, infra::Colour colour, infra::Region boundingBox);
        void DrawVerticalLine(infra::Point from, uint16_t deltaY, infra::Colour colour);
        infra::Point DrawCharacter(infra::Point position, char c, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox);
        void DrawPixel(infra::Point position, infra::Colour colour);

    private:
        infra::Vector size;
        std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> window;
        std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>> renderer;
    };
}

#endif
