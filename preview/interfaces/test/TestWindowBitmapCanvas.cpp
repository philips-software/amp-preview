#include "preview/interfaces/WindowBitmapCanvas.hpp"
#include "preview/interfaces/test_doubles/BitmapPainterMock.hpp"
#include "gmock/gmock.h"

class WindowBitmapCanvasTest
    : public testing::Test
{
public:
    testing::StrictMock<hal::BitmapPainterMock> bitmapPainter;
    infra::Bitmap::Rgb565<30, 20> bitmap;
};

TEST_F(WindowBitmapCanvasTest, size_is_returned)
{
    services::WindowBitmapCanvas canvas1(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);
    EXPECT_EQ(infra::Vector(40, 60), canvas1.Size());
}

TEST_F(WindowBitmapCanvasTest, DrawPixel_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawPixel(testing::Ref(bitmap), infra::Point(12, 8), infra::Colour::blue, infra::Region(infra::Point(20, 10), bitmap.size - infra::Vector(20, 10))));
    canvas.DrawPixel(infra::Point(2, 3), infra::Colour::blue, infra::Region(infra::Point(10, 5), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawLine_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawLine(testing::Ref(bitmap), infra::Point(12, 8), infra::Point(14, 10), infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawLine(infra::Point(2, 3), infra::Point(4, 5), infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawRectangle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawRectangle(testing::Ref(bitmap), infra::Region(infra::Point(12, 8), infra::Vector(4, 5)), infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawRectangle(infra::Region(infra::Point(2, 3), infra::Vector(4, 5)), infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawFilledRectangle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawFilledRectangle(testing::Ref(bitmap), infra::Region(infra::Point(12, 8), infra::Vector(4, 5)), infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawFilledRectangle(infra::Region(infra::Point(2, 3), infra::Vector(4, 5)), infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawRoundedRectangle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawRoundedRectangle(testing::Ref(bitmap), infra::Region(infra::Point(12, 8), infra::Vector(4, 5)), 2, infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawRoundedRectangle(infra::Region(infra::Point(2, 3), infra::Vector(4, 5)), 2, infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawFilledRoundedRectangle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawFilledRoundedRectangle(testing::Ref(bitmap), infra::Region(infra::Point(12, 8), infra::Vector(4, 5)), 2, infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawFilledRoundedRectangle(infra::Region(infra::Point(2, 3), infra::Vector(4, 5)), 2, infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawCircle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawCircle(testing::Ref(bitmap), infra::Point(12, 8), 2, infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawCircle(infra::Point(2, 3), 2, infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawFilledCircle_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);

    EXPECT_CALL(bitmapPainter, DrawFilledCircle(testing::Ref(bitmap), infra::Point(12, 8), 2, infra::Colour::blue, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawFilledCircle(infra::Point(2, 3), 2, infra::Colour::blue, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawBitmap_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);
    infra::Bitmap::Rgb565<10, 10> sourceBitmap;

    EXPECT_CALL(bitmapPainter, DrawBitmap(testing::Ref(bitmap), infra::Point(12, 8), testing::Ref(sourceBitmap), infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawBitmap(infra::Point(2, 3), sourceBitmap, infra::Region(infra::Point(), canvas.Size()));
}

TEST_F(WindowBitmapCanvasTest, DrawString_is_shifted)
{
    services::WindowBitmapCanvas canvas(infra::Vector(40, 60), bitmap, infra::Vector(10, 5), bitmapPainter);
    infra::Font font;

    EXPECT_CALL(bitmapPainter, DrawString(testing::Ref(bitmap), infra::Point(12, 8), "abcd", testing::Ref(font), infra::Colour::blue, infra::RightAngle::angle_0, infra::Region(infra::Point(10, 5), bitmap.size - infra::Vector(10, 5))));
    canvas.DrawString(infra::Point(2, 3), "abcd", font, infra::Colour::blue, infra::RightAngle::angle_0, infra::Region(infra::Point(), canvas.Size()));
}
