#ifndef PREVIEW_TOUCH_RECIPIENT_MOCK_HPP
#define PREVIEW_TOUCH_RECIPIENT_MOCK_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"

namespace services
{
    class TouchRecipientMock
        : public TouchRecipient::WithView<ViewMock>
    {
    public:
        using TouchRecipient::WithView<ViewMock>::WithView;

        MOCK_METHOD1(StartTouch, void(infra::Point point));
        MOCK_METHOD1(DragIn, void(infra::Point point));
        MOCK_METHOD1(DragTo, void(infra::Point point));
        MOCK_METHOD0(DragOut, void());
        MOCK_METHOD0(StopTouch, void());
        MOCK_METHOD1(Swipe, void(Direction direction));
    };
}

#endif
