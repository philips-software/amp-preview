#include "preview/views/ViewButtonPanel.hpp"

namespace services
{
    ViewButtonPanel::ViewButtonPanel(infra::Colour inactiveColour, infra::Colour activeColour)
        : ViewPanel(inactiveColour)
        , inactiveColour(inactiveColour)
        , activeColour(activeColour)
    {}

    void ViewButtonPanel::Activate()
    {
        SetColour(activeColour);
    }

    void ViewButtonPanel::Deactivate()
    {
        SetColour(inactiveColour);
    }
}
