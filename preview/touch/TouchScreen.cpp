#include "preview/touch/TouchScreen.hpp"

namespace services
{
    TouchScreen::TouchScreen(hal::GpioPin& xPlus, hal::GpioPin& xMinus, hal::GpioPin& yPlus, hal::GpioPin& yMinus,
        infra::CreatorBase<AnalogToDigitalPin, void()>& xPlusAnalogPin, infra::CreatorBase<AnalogToDigitalPin, void()>& yPlusAnalogPin, uint32_t touchDelta)
        : xPlus(xPlus)
        , xMinus(xMinus)
        , yPlus(yPlus)
        , yMinus(yMinus)
        , xPlusAnalogPin(xPlusAnalogPin)
        , yPlusAnalogPin(yPlusAnalogPin)
        , touchDelta(touchDelta)
    {}

    void TouchScreen::Measure(const infra::Function<void(infra::Optional<infra::Point> position)>& onTouched)
    {
        assert(this->onTouched == nullptr);
        this->onTouched = onTouched;

        state.Emplace<StateTouchMeasurement>(*this, false);
    }

    TouchScreen::StateTouchMeasurement::StateTouchMeasurement(TouchScreen& touchScreen, bool finalMeasurement)
        : touchScreen(touchScreen)
        , xMinus(touchScreen.xMinus, true)
        , yMinus(touchScreen.yMinus, false)
        , xPlusAnalogPin(touchScreen.xPlusAnalogPin)
        , yPlusAnalogPin(touchScreen.yPlusAnalogPin)
        , startConversion(std::chrono::microseconds(100), [this]()
              {
                  xPlusAnalogPin->Measure(infra::MakeRangeFromSingleObject(this->pixelPosition), [this]()
                      {
                          OnMeasurementXDone(pixelPosition.Value());
                      });
              })
        , finalMeasurement(finalMeasurement)
    {}

    void TouchScreen::StateTouchMeasurement::OnMeasurementXDone(uint32_t xTouchResult)
    {
        this->xTouchResult = xTouchResult;
        yPlusAnalogPin->Measure(infra::MakeRangeFromSingleObject(this->pixelPosition), [this]()
            {
                OnMeasurementYDone(pixelPosition.Value());
            });
    }

    void TouchScreen::StateTouchMeasurement::OnMeasurementYDone(uint32_t yTouchResult)
    {
        if (xTouchResult >= yTouchResult + touchScreen.touchDelta)
        {
            touchScreen.onTouched(infra::none);
            touchScreen.state.Emplace<StateIdle>();
        }
        else if (finalMeasurement)
        {
            touchScreen.onTouched(infra::MakeOptional(infra::Point(touchScreen.xResult, touchScreen.yResult)));
            touchScreen.state.Emplace<StateIdle>();
        }
        else
            touchScreen.state.Emplace<StateXMeasurement>(touchScreen);
    }

    TouchScreen::StateXMeasurement::StateXMeasurement(TouchScreen& touchScreen)
        : touchScreen(touchScreen)
        , yPlus(touchScreen.yPlus, true)
        , yMinus(touchScreen.yMinus, false)
        , xPlusAnalogPin(touchScreen.xPlusAnalogPin)
        , startConversion(std::chrono::microseconds(100), [this]()
              {
                  xPlusAnalogPin->Measure(infra::MakeRangeFromSingleObject(this->pixelPosition), [this]()
                      {
                          OnMeasurementDone(pixelPosition.Value());
                      });
              })
    {}

    void TouchScreen::StateXMeasurement::OnMeasurementDone(uint32_t pixelPosition)
    {
        touchScreen.xResult = pixelPosition;
        touchScreen.state.Emplace<StateYMeasurement>(touchScreen);
    }

    TouchScreen::StateYMeasurement::StateYMeasurement(TouchScreen& touchScreen)
        : touchScreen(touchScreen)
        , xPlus(touchScreen.xPlus, true)
        , xMinus(touchScreen.xMinus, false)
        , yPlusAnalogPin(touchScreen.yPlusAnalogPin)
        , startConversion(std::chrono::microseconds(100), [this]()
              {
                  yPlusAnalogPin->Measure(infra::MakeRangeFromSingleObject(this->pixelPosition), [this]()
                      {
                          OnMeasurementDone(this->pixelPosition.Value());
                      });
              })
    {}

    void TouchScreen::StateYMeasurement::OnMeasurementDone(uint32_t pixelPosition)
    {
        touchScreen.yResult = pixelPosition;
        touchScreen.state.Emplace<StateTouchMeasurement>(touchScreen, true);
    }
}
