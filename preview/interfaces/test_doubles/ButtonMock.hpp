#ifndef PREVIEW_BUTTON_MOCK_HPP
#define PREVIEW_BUTTON_MOCK_HPP

#include "preview/interfaces/Button.hpp"
#include "preview/interfaces/test_doubles/ViewMock.hpp"
#include "gmock/gmock.h"

namespace services
{
    class ButtonMock
        : public Button
    {
    public:
        MOCK_METHOD0(Activate, void());
        MOCK_METHOD0(Deactivate, void());
    };

    class ViewButtonMock
        : public ButtonMock
        , public ViewMock
    {
    public:
        using ViewMock::ViewMock;
    };
}

#endif
