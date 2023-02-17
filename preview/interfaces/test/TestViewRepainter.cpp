#include "gmock/gmock.h"
#include "infra/event/test_helper/EventDispatcherFixture.hpp"
#include "preview/interfaces/ViewRepainter.hpp"
#include "preview/interfaces/test_doubles/ViewPainterMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

namespace
{
    class ViewRepainterPaintWhenDirtyStub
        : public services::ViewRepainterPaintWhenDirty
    {
    public:
        using services::ViewRepainterPaintWhenDirty::ViewRepainterPaintWhenDirty;

        using services::ViewRepainterPaintWhenDirty::Dirty;
    };
}

class ViewRepainterPaintWhenDirtyTest
    : public testing::Test
    , public infra::EventDispatcherFixture
{
public:
    ViewRepainterPaintWhenDirtyTest()
        : repainter(painter, view)
        , view(infra::Vector(10, 20))
    {}

public:
    testing::StrictMock<services::ViewMock> view;
    testing::StrictMock<services::ViewPainterMock> painter;
    ViewRepainterPaintWhenDirtyStub repainter;

    infra::Region region{ 1, 2, 3, 4 };
};

TEST_F(ViewRepainterPaintWhenDirtyTest, paints_when_requested)
{
    repainter.Dirty(region);

    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_));
    ExecuteAllActions();
}

TEST_F(ViewRepainterPaintWhenDirtyTest, paints_once_on_double_request)
{
    repainter.Dirty(region);
    repainter.Dirty(region);

    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_));
    ExecuteAllActions();
}

TEST_F(ViewRepainterPaintWhenDirtyTest, paints_when_dirty_and_previous_paint_finished)
{
    repainter.Dirty(region);
    infra::Function<void()> onDone;
    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_)).WillOnce(testing::SaveArg<2>(&onDone));
    ExecuteAllActions();

    repainter.Dirty(region);
    onDone();

    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_));
    ExecuteAllActions();
}

TEST_F(ViewRepainterPaintWhenDirtyTest, will_not_continuously_repaint)
{
    repainter.Dirty(region);
    infra::Function<void()> onDone;
    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_)).WillOnce(testing::SaveArg<2>(&onDone));
    ExecuteAllActions();

    repainter.Dirty(region);
    onDone();

    EXPECT_CALL(painter, Paint(testing::Ref(view), region, testing::_)).WillOnce(testing::SaveArg<2>(&onDone));
    ExecuteAllActions();

    onDone();
    ExecuteAllActions();
}
