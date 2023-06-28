#ifndef PREVIEW_VIEW_ROTATING_HPP
#define PREVIEW_VIEW_ROTATING_HPP

#include "preview/interfaces/Colour.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewRotating
        : public View
    {
    public:
        template<class T>
        class WithView;

        explicit ViewRotating(infra::RightAngle angle);

        // Implementation of View
        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetAngle(infra::RightAngle newAngle);

    protected:
        void ViewRegionChanged() override;
        void Dirty(infra::Region region) override;

    private:
        class CanvasRotating
            : public hal::Canvas
        {
        public:
            CanvasRotating(hal::Canvas& canvas, ViewRotating& view);

            // Implementation of Canvas
            infra::Vector Size() const override;
            void DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox) override;
            void DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override;
            void DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
            void DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
            void DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
            void DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
            void DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
            void DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override;
            void DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
            void DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override;
            void DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override;
            void DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override;
            void DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override;

        private:
            infra::Point Rotated(infra::Point point) const;
            infra::Region Rotated(infra::Region region) const;

        private:
            hal::Canvas& canvas;
            ViewRotating& view;
        };

    private:
        services::View* subView = nullptr;
        infra::RightAngle angle;
        infra::Region rotatedRegion;
    };

    template<class T>
    class ViewRotating::WithView
        : public ViewRotating
    {
    public:
        template<class... Args>
        WithView(infra::RightAngle angle, Args&&... args)
            : ViewRotating(angle)
            , subView(std::forward<Args>(args)...)
        {
            SetSubView(subView);
        }

        T& SubView()
        {
            return subView;
        }

    private:
        T subView;
    };
}

#endif
