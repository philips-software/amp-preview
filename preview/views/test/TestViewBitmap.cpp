#include "gmock/gmock.h"
#include "preview/views/ViewBitmap.hpp"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

class ViewBitmapTest
    : public testing::Test
{
public:
    ViewBitmapTest()
        : viewBitmap(bitmap)
    {}

    testing::StrictMock<hal::CanvasMock> canvas;
    infra::Bitmap::Rgb565<30, 20> bitmap;
    services::ViewBitmap viewBitmap;
    testing::StrictMock<services::TopViewMock> topView{ viewBitmap };
};

TEST_F(ViewBitmapTest, paint_bitmap)
{
    infra::Region boundingRegion;
    EXPECT_CALL(canvas, DrawBitmap(infra::Point(), bitmap, infra::Region()));
    viewBitmap.Paint(canvas, boundingRegion);
}
