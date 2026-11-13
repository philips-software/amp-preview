#include "preview/stm32fxxx/BitmapPainterStm.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include "preview/interfaces/BitmapPainter.hpp"

namespace hal
{
    namespace
    {
        const std::array<uint32_t, 2> pixelFormatToDma2dColour = {
            DMA2D_ARGB8888,
            DMA2D_RGB565
        };
    }

    BitmapPainterStm::BitmapPainterStm()
    {
        __HAL_RCC_DMA2D_CLK_ENABLE();
    }

    BitmapPainterStm::~BitmapPainterStm()
    {
        __HAL_RCC_DMA2D_CLK_DISABLE();
    }

    void BitmapPainterStm::DrawPixel(infra::Bitmap& bitmap, infra::Point position, infra::Colour colour, infra::Region boundingBox)
    {
        if (boundingBox.Contains(position))
        {
            WaitUntilDrawingFinished();
            BitmapPainterCanonical::DrawPixel(bitmap, position, colour);
        }
    }

    void BitmapPainterStm::DrawFilledRectangle(infra::Bitmap& bitmap, infra::Region position, infra::Colour colour, infra::Region boundingBox)
    {
        if (bitmap.isSimple)
        {
            auto& simpleBitmap = static_cast<infra::SimpleBitmap&>(bitmap);
            infra::Region destination = position & boundingBox;

            if (!destination.Empty())
            {
                WaitUntilDrawingFinished();

                DMA2D->OMAR = reinterpret_cast<uint32_t>(simpleBitmap.BufferAddress(destination.TopLeft()));
                DMA2D->OOR = simpleBitmap.size.deltaX - destination.Width();
                DMA2D->OPFCCR = pixelFormatToDma2dColour[static_cast<uint32_t>(simpleBitmap.pixelFormat)];
                DMA2D->OCOLR = infra::ConvertRgb888To(colour, simpleBitmap.pixelFormat);

                DMA2D->NLR = destination.Height() | (destination.Width() << POSITION_VAL(DMA2D_NLR_PL));
                DMA2D->CR = DMA2D_R2M | DMA2D_CR_START;
            }
        }
        else
            BitmapPainterCanonical::DrawFilledRectangle(bitmap, position, colour, boundingBox);
    }

    void BitmapPainterStm::DrawBitmap(infra::Bitmap& bitmap, infra::Point position, const infra::SimpleBitmap& sourceBitmap, infra::Region boundingBox)
    {
        if (bitmap.isSimple)
        {
            auto& simpleBitmap = static_cast<infra::SimpleBitmap&>(bitmap);
            infra::Region destination = infra::Region(position, sourceBitmap.size) & boundingBox;

            if (!destination.Empty())
            {
                infra::Vector bitmapShift = destination.TopLeft() - position;
                WaitUntilDrawingFinished();

                DMA2D->FGMAR = reinterpret_cast<uint32_t>(sourceBitmap.BufferAddress(infra::Point() + bitmapShift));
                DMA2D->FGOR = sourceBitmap.size.deltaX - destination.Width();
                DMA2D->FGPFCCR = pixelFormatToDma2dColour[static_cast<uint32_t>(sourceBitmap.pixelFormat)];

                DMA2D->OMAR = reinterpret_cast<uint32_t>(simpleBitmap.BufferAddress(destination.TopLeft()));
                DMA2D->OOR = simpleBitmap.size.deltaX - destination.Width();
                DMA2D->OPFCCR = pixelFormatToDma2dColour[static_cast<uint32_t>(simpleBitmap.pixelFormat)];

                DMA2D->NLR = destination.Height() | (destination.Width() << POSITION_VAL(DMA2D_NLR_PL));
                DMA2D->CR = (sourceBitmap.pixelFormat == simpleBitmap.pixelFormat ? DMA2D_M2M : DMA2D_M2M_PFC) | DMA2D_CR_START;
            }
        }
        else
            BitmapPainterCanonical::DrawBitmap(bitmap, position, sourceBitmap, boundingBox);
    }

    void BitmapPainterStm::WaitUntilDrawingFinished() const
    {
        while ((DMA2D->CR & DMA2D_CR_START) != RESET)
        {
        }
    }

    void BitmapPainterStm::DrawHorizontalLine(infra::SimpleBitmap& bitmap, infra::Point from, uint16_t deltaX, infra::Colour colour)
    {
        if (deltaX > 0)
        {
            WaitUntilDrawingFinished();

            DMA2D->OMAR = reinterpret_cast<uint32_t>(bitmap.BufferAddress(from));
            DMA2D->OOR = bitmap.size.deltaX - deltaX;
            DMA2D->OPFCCR = pixelFormatToDma2dColour[static_cast<uint32_t>(bitmap.pixelFormat)];
            DMA2D->OCOLR = infra::ConvertRgb888To(colour, bitmap.pixelFormat);

            DMA2D->NLR = 1 | (deltaX << POSITION_VAL(DMA2D_NLR_PL));
            DMA2D->CR = DMA2D_R2M | DMA2D_CR_START;
        }
    }

    void BitmapPainterStm::DrawVerticalLine(infra::SimpleBitmap& bitmap, infra::Point from, uint16_t deltaY, infra::Colour colour)
    {
        if (deltaY > 0)
        {
            WaitUntilDrawingFinished();

            DMA2D->OMAR = reinterpret_cast<uint32_t>(bitmap.BufferAddress(from));
            DMA2D->OOR = bitmap.size.deltaX - 1;
            DMA2D->OPFCCR = pixelFormatToDma2dColour[static_cast<uint32_t>(bitmap.pixelFormat)];
            DMA2D->OCOLR = infra::ConvertRgb888To(colour, bitmap.pixelFormat);

            DMA2D->NLR = deltaY | (1 << POSITION_VAL(DMA2D_NLR_PL));
            DMA2D->CR = DMA2D_R2M | DMA2D_CR_START;
        }
    }
}
