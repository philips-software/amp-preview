#include "preview/views/ViewRotating.hpp"

namespace services
{
    ViewRotating::ViewRotating(infra::RightAngle angle)
        : angle(angle)
    {}

    infra::Vector ViewRotating::MinimumSize() const
    {
        if (subView)
            return infra::RotatedVectorInRegion(subView->MinimumSize(), angle, rotatedRegion);
        else
            return infra::Vector();
    }

    infra::Vector ViewRotating::MaximumSize() const
    {
        if (subView)
            return infra::RotatedVectorInRegion(subView->MaximumSize(), angle, rotatedRegion);
        else
            return infra::Vector::Maximum();
    }

    void ViewRotating::Paint(hal::Canvas& canvas, infra::Region boundingRegion)
    {
        if (subView != nullptr)
        {
            CanvasRotating rotating(canvas, *this);
            subView->Paint(rotating, rotatedRegion);
        }
    }

    void ViewRotating::ResetSize()
    {
        if (subView)
        {
            subView->ResetSize();
            rotatedRegion = infra::RotatedRegionInRegion(subView->ViewRegion(), -angle, subView->ViewRegion());
            ResizeWithoutTrigger(rotatedRegion.Size());
        }
        else
            ResizeWithoutTrigger(infra::Vector());
    }

    void ViewRotating::SetSubView(View& newSubView)
    {
        subView = &newSubView;
        subView->SetParent(*this);
        Dirty(infra::RotatedRegionInRegion(subView->ViewRegion(), angle, rotatedRegion));
    }

    void ViewRotating::SetAngle(infra::RightAngle newAngle)
    {
        angle = newAngle;
        rotatedRegion = infra::RotatedRegionInRegion(ViewRegion(), -angle, ViewRegion());
        Dirty(ViewRegion());
    }

    void ViewRotating::ViewRegionChanged()
    {
        rotatedRegion = infra::RotatedRegionInRegion(ViewRegion(), -angle, ViewRegion());
        if (subView != nullptr)
            subView->SetViewRegion(rotatedRegion);
    }

    void ViewRotating::Dirty(infra::Region region)
    {
        View::Dirty(infra::RotatedRegionInRegion(region, angle, rotatedRegion));
    }

    ViewRotating::CanvasRotating::CanvasRotating(hal::Canvas& canvas, ViewRotating& view)
        : canvas(canvas)
        , view(view)
    {}

    infra::Vector ViewRotating::CanvasRotating::Size() const
    {
        return canvas.Size();
    }

    void ViewRotating::CanvasRotating::DrawPixel(infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawPixel(Rotated(position), colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawLine(infra::Point from, infra::Point to, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawLine(Rotated(from), Rotated(to), colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawRectangle(Rotated(position), colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawFilledRectangle(infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawFilledRectangle(Rotated(position), colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawRoundedRectangle(Rotated(position), r, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawFilledRoundedRectangle(infra::Region position, uint16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawFilledRoundedRectangle(Rotated(position), r, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawCircle(Rotated(centre), r, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawCircle(infra::Point centre, int16_t rSmall, int16_t rLarge, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawCircle(Rotated(centre), rSmall, rLarge, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawFilledCircle(infra::Point centre, int16_t r, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawFilledCircle(Rotated(centre), r, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Region boundingBox)
    {
        canvas.DrawBitmap(Rotated(infra::Region(position, sourceBitmap.size)).TopLeft(), sourceBitmap, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawTransparentBitmap(infra::Point position, const infra::Bitmap& sourceBitmap, uint32_t transparencyColour, infra::Region boundingBox)
    {
        canvas.DrawTransparentBitmap(Rotated(infra::Region(position, sourceBitmap.size)).TopLeft(), sourceBitmap, transparencyColour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawIcon(infra::Point position, const infra::Bitmap& sourceBitmap, infra::Colour colour, infra::Region boundingBox)
    {
        canvas.DrawIcon(Rotated(infra::Region(position, sourceBitmap.size)).TopLeft(), sourceBitmap, colour, Rotated(boundingBox));
    }

    void ViewRotating::CanvasRotating::DrawString(infra::Point position, infra::BoundedConstString string, const infra::Font& font, infra::Colour colour, infra::RightAngle direction, infra::Region boundingBox)
    {
        canvas.DrawString(Rotated(position), string, font, colour, direction + view.angle, Rotated(boundingBox));
    }

    infra::Point ViewRotating::CanvasRotating::Rotated(infra::Point point) const
    {
        return infra::RotatedPointInRegion(point, view.angle, view.rotatedRegion);
    }

    infra::Region ViewRotating::CanvasRotating::Rotated(infra::Region region) const
    {
        return infra::RotatedRegionInRegion(region, view.angle, view.rotatedRegion);
    }
}
