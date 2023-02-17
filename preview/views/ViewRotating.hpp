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

        ViewRotating(infra::RightAngle angle);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

        void SetSubView(View& newSubView);
        void SetAngle(infra::RightAngle newAngle);

    protected:
        virtual void ViewRegionChanged() override;
        virtual void Dirty(infra::Region region) override;

    private:
        class CanvasRotating
            : public hal::Canvas
        {
        public:
            CanvasRotating(hal::Canvas& canvas, ViewRotating& view);

            // Implementation of Canvas
            virtual infra::Vector Size() const override;
            virtual void DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox) override;
            virtual void DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox) override;
            virtual void DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox) override;
            virtual void DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox) override;

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
