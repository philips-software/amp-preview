#include "preview/touch/TouchSpinInteger.hpp"
#include "infra/stream/StringOutputStream.hpp"

namespace services
{
    TouchSpinInteger::TouchSpinInteger(services::View& view, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement)
        : view(view)
        , value(start)
        , from(from)
        , to(to)
        , circular(circular)
        , distancePerIncrement(distancePerIncrement)
    {}

    TouchSpinInteger::TouchSpinInteger(services::View& view, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width)
        : view(view)
        , value(start)
        , from(from)
        , to(to)
        , circular(circular)
        , distancePerIncrement(distancePerIncrement)
        , width(std::make_optional(width))
    {}

    void TouchSpinInteger::StartTouch(infra::Point point)
    {
        startTouch = point;
        stepsReported = 0;
    }

    void TouchSpinInteger::DragIn(infra::Point point)
    {
        StartTouch(point);
    }

    void TouchSpinInteger::DragTo(infra::Point point)
    {
        auto steps = (startTouch->y - point.y) / distancePerIncrement - stepsReported;
        stepsReported += steps;

        if (steps != 0)
            Report(steps, steps > 0 ? Direction::down : Direction::up);
    }

    void TouchSpinInteger::DragOut()
    {
        StopTouch();
    }

    void TouchSpinInteger::StopTouch()
    {
        startTouch.reset();
    }

    void TouchSpinInteger::Swipe(Direction direction)
    {}

    View& TouchSpinInteger::GetView()
    {
        return view;
    }

    int32_t TouchSpinInteger::Value() const
    {
        return value;
    }

    void TouchSpinInteger::SetValue(int32_t newValue)
    {
        auto oldValue = value;
        value = AdjustedValue(newValue);

        if (value != oldValue)
        {
            WriteValueString();
            SetText(valueString);
        }
    }

    void TouchSpinInteger::SetValue(int32_t newValue, Direction fromDirection)
    {
        auto oldValue = value;
        value = AdjustedValue(newValue);

        if (value != oldValue)
        {
            WriteValueString();
            SetText(valueString, fromDirection);
        }
    }

    void TouchSpinInteger::SetInitialValue()
    {
        WriteValueString();
        SetText(valueString);
    }

    void TouchSpinInteger::Report(int32_t steps, Direction from)
    {
        SetValue(value + steps, from);
    }

    void TouchSpinInteger::WriteValueString()
    {
        valueString.clear();
        infra::StringOutputStream stream(valueString);
        if (width != std::nullopt)
            stream << infra::Width(*width, '0') << value;
        else
            stream << value;
    }

    int32_t TouchSpinInteger::AdjustedValue(int32_t newValue) const
    {
        if (circular)
            return Modulus(newValue - from, to - from + 1) + from;
        else
            return std::max(std::min(newValue, to), from);
    }

    int32_t TouchSpinInteger::Modulus(int32_t a, int32_t b) const
    {
        auto result = a % b;
        if (result < 0)
            result += b;
        return result;
    }

    TouchSpinInteger::WithViewText::WithViewText(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, TextAttributes attributes)
        : ViewText(attributes)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement)
    {
        SetInitialValue();
    }

    TouchSpinInteger::WithViewText::WithViewText(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, TextAttributes attributes)
        : ViewText(attributes)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement, width)
    {
        SetInitialValue();
    }

    void TouchSpinInteger::WithViewText::SetText(infra::BoundedConstString text)
    {
        ViewText::SetText(valueString);
    }

    void TouchSpinInteger::WithViewText::SetText(infra::BoundedConstString text, Direction from)
    {
        ViewText::SetText(valueString);
    }

    TouchSpinInteger::WithViewFadingText::WithViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, FadingTextAttributes attributes)
        : ViewFadingText(buffer1, buffer2, attributes)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement)
    {
        SetInitialValue();
        SetTextAndResize(Text());
    }

    TouchSpinInteger::WithViewFadingText::WithViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, FadingTextAttributes attributes)
        : ViewFadingText(buffer1, buffer2, attributes)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement, width)
    {
        SetInitialValue();
        SetTextAndResize(Text());
    }

    void TouchSpinInteger::WithViewFadingText::SetText(infra::BoundedConstString text)
    {
        ViewFadingText::SetText(valueString);
    }

    void TouchSpinInteger::WithViewFadingText::SetText(infra::BoundedConstString text, Direction from)
    {
        ViewFadingText::FadeIn(valueString, from);
    }
}
