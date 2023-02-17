#include "gmock/gmock.h"
#include "preview/interfaces/test_doubles/CanvasMock.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "preview/views/ViewSelector.hpp"

class SelectorTest
    : public testing::Test
{
public:
    SelectorTest()
        : view1(infra::Vector(20, 10))
        , view2(infra::Vector(40, 30))
        , view3(infra::Vector(40, 30))
    {
        EXPECT_CALL(view1, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(5, 5)));
        EXPECT_CALL(view1, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(20, 20)));
        EXPECT_CALL(view2, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(10, 10)));
        EXPECT_CALL(view2, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(30, 30)));
        EXPECT_CALL(view3, MinimumSize()).WillRepeatedly(testing::Return(infra::Vector(15, 15)));
        EXPECT_CALL(view3, MaximumSize()).WillRepeatedly(testing::Return(infra::Vector(40, 40)));
    }

    testing::StrictMock<services::ViewMock> view1;
    testing::StrictMock<services::ViewMock> view2;
    testing::StrictMock<services::ViewMock> view3;
    services::ViewSelector::WithMaxViews<5> view;
    testing::StrictMock<hal::CanvasMock> canvas;
    testing::StrictMock<services::TopViewMock> topView{ view };
};

TEST_F(SelectorTest, PaintWithoutViews)
{
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}

TEST_F(SelectorTest, PaintWithViews)
{
    view.Add(view1);
    view.Add(view2);

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));

    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_CALL(view2, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}

TEST_F(SelectorTest, MinimumSize)
{   
    EXPECT_EQ(infra::Vector(0, 0), view.MinimumSize());

    view.Add(view1);
    view.Add(view2);
    EXPECT_EQ(infra::Vector(5, 5), view.MinimumSize());
    
    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_EQ(infra::Vector(10, 10), view.MinimumSize());

    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_EQ(infra::Vector(5, 5), view.MinimumSize());
}

TEST_F(SelectorTest, MaximumSize)
{
    EXPECT_EQ(infra::Vector(32767, 32767), view.MaximumSize());

    view.Add(view1);
    view.Add(view2);
    EXPECT_EQ(infra::Vector(20, 20), view.MaximumSize());

    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_EQ(infra::Vector(30, 30), view.MaximumSize());

    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_EQ(infra::Vector(20, 20), view.MaximumSize());
}

TEST_F(SelectorTest, ResetSize_resets_subview_size)
{
    view.Add(view1);

    EXPECT_CALL(view1, ViewRegionChanged());
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(150, 200))));
    view1.Resize(infra::Vector(150, 200));

    EXPECT_CALL(view1, ResetSize());
    view.ResetSize();

    EXPECT_EQ(infra::Region(infra::Point(), infra::Vector(150, 200)), view.ViewRegion());
}

TEST_F(SelectorTest, Previous)
{
    view.Add(view1);
    view.Add(view2);
    view.Add(view3);

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));

    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Next();
    EXPECT_CALL(topView, Dirty(infra::Region()));
    view.Previous();

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}

TEST_F(SelectorTest, Remove)
{
    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Resize(infra::Vector(80, 100));
    view.Add(view1);
    view.Add(view2);
    view.Add(view3);

    EXPECT_CALL(view1, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));

    EXPECT_CALL(topView, Dirty(infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Remove(view1);

    EXPECT_CALL(view2, Paint(testing::Ref(canvas), infra::Region(infra::Point(), infra::Vector(80, 100))));
    view.Paint(canvas, infra::Region(infra::Point(), infra::Vector(80, 100)));
}
