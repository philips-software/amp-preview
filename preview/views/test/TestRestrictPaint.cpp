#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/RestrictPaint.hpp"
#include "gmock/gmock.h"

class RestrictPaintTest
    : public testing::Test
{
public:
    RestrictPaintTest()
        : view(infra::Vector(10, 20))
    {}

    testing::StrictMock<hal::CanvasMock> canvas;
    services::RestrictPaint<services::ViewMock> view;
};

TEST_F(RestrictPaintTest, Paint_is_restricted)
{
    EXPECT_CALL(view, Paint(testing::Ref(canvas), infra::Region(infra::Point(5, 0), infra::Vector(5, 20))));
    view.Paint(canvas, infra::Region(infra::Point(5, -1), infra::Vector(40, 40)));
}
