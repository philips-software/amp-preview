#include "examples/clicking_scrolling/TouchViewClickingScrolling.hpp"

namespace application
{
    TouchViewClickingScrolling::TouchViewClickingScrolling()
        : services::TouchPanel(infra::Colour::white)
        , layout(20)
        , scrollingText(std::chrono::milliseconds(250), 4, std::chrono::seconds(2), services::TextAttributes{ infra::Colour::blue }, "This is an example of scrolling text. It continuously scrolls to the left until it is reset.")
        , button([this]()
              { OnTouched(); },
              services::FramedTextButtonAttributes{ infra::Colour::red, infra::Colour::white, infra::Colour::lightGray, infra::Colour::blue }, "Change text colour")
    {
        SetSubView(layout);
        layout.AddDummy(1);
        layout.GetView().AddFill(scrollingText.Scrolling());
        layout.Add(button, 1, services::HorizontalAlignment::centre);
        layout.AddDummy(1);
    }

    void TouchViewClickingScrolling::OnTouched()
    {
        scrollingText.SubView().SetTextColour(scrollingText.SubView().Attributes().colour == infra::Colour::red ? infra::Colour::blue : infra::Colour::red);
    }
}
