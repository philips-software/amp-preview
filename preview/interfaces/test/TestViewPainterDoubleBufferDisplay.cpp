#include "infra/event/test_helper/EventDispatcherFixture.hpp"
#include "preview/interfaces/ViewPainterDoubleBufferDisplay.hpp"
#include "preview/interfaces/test_doubles/BitmapPainterMock.hpp"
#include "preview/interfaces/test_doubles/DoubleBufferDisplayMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "gmock/gmock.h"

class ViewPainterDoubleBufferDisplayTest
    : public testing::Test
    , public infra::EventDispatcherFixture
{
public:
    ViewPainterDoubleBufferDisplayTest()
        : viewPainter(display, bitmapPainter)
        , view(infra::Vector(4, 8))
    {}

    testing::StrictMock<hal::DoubleBufferDisplayMock> display;
    testing::StrictMock<hal::BitmapPainterMock> bitmapPainter;
    testing::StrictMock<services::ViewMock> view;
    services::ViewPainterDoubleBufferDisplay viewPainter;
};

TEST_F(ViewPainterDoubleBufferDisplayTest, Paint)
{
    testing::InSequence s;

    infra::Region region(infra::Point(), infra::Vector(4, 8));

    infra::Bitmap::Rgb565<4, 8> bitmap;
    EXPECT_CALL(display, DrawingBitmap()).WillOnce(testing::ReturnRef(bitmap));
    EXPECT_CALL(view, Paint(testing::_, region));
    EXPECT_CALL(bitmapPainter, WaitUntilDrawingFinished());
    infra::Function<void()> onDone;
    EXPECT_CALL(display, SwapLayers(testing::_)).WillOnce(testing::SaveArg<0>(&onDone));

    infra::Bitmap::Rgb565<4, 8> drawingBitmap;
    EXPECT_CALL(display, DrawingBitmap()).WillOnce(testing::ReturnRef(drawingBitmap));
    infra::Bitmap::Rgb565<4, 8> viewingBitmap;
    EXPECT_CALL(display, ViewingBitmap()).WillOnce(testing::ReturnRef(viewingBitmap));
    EXPECT_CALL(bitmapPainter, DrawBitmap(testing::Ref(drawingBitmap), infra::Point(), testing::Ref(viewingBitmap), region));
    viewPainter.Paint(view, region, infra::emptyFunction);

    onDone();
}
