#ifndef PREVIEW_TOUCH_SCREEN_HPP
#define PREVIEW_TOUCH_SCREEN_HPP

#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/Optional.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "preview/interfaces/Geometry.hpp"

namespace services
{
    class TouchScreen
    {
    public:
        using PixelPosition = infra::Quantity<infra::Identity, uint16_t>;
        using AnalogToDigitalPin = hal::AnalogToDigitalPin<infra::Identity, uint16_t>;

        TouchScreen(hal::GpioPin& xPlus, hal::GpioPin& xMinus, hal::GpioPin& yPlus, hal::GpioPin& yMinus, infra::CreatorBase<AnalogToDigitalPin, void()>& xPlusAnalogPin, infra::CreatorBase<AnalogToDigitalPin, void()>& yPlusAnalogPin, uint32_t touchDelta);

        void Measure(const infra::Function<void(std::optional<infra::Point> position)>& onTouched);

    private:
        class State
        {
        public:
            State() = default;
            State(const State& other) = delete;
            State& operator=(const State& other) = delete;
            virtual ~State() = default;
        };

        class StateIdle
            : public State
        {};

        class StateTouchMeasurement
            : public State
        {
        public:
            StateTouchMeasurement(TouchScreen& touchScreen, bool finalMeasurement);

        private:
            void OnMeasurementXDone(uint32_t xTouchResult);
            void OnMeasurementYDone(uint32_t yTouchResult);

        private:
            TouchScreen& touchScreen;
            hal::OutputPin xMinus;
            hal::OutputPin yMinus;
            infra::ProxyCreator<AnalogToDigitalPin, void()> xPlusAnalogPin;
            infra::ProxyCreator<AnalogToDigitalPin, void()> yPlusAnalogPin;
            infra::TimerSingleShot startConversion;
            bool finalMeasurement;
            uint32_t xTouchResult;
            PixelPosition pixelPosition;
        };

        class StateXMeasurement
            : public State
        {
        public:
            explicit StateXMeasurement(TouchScreen& touchScreen);

        private:
            void OnMeasurementDone(uint32_t pixelPosition);

        private:
            TouchScreen& touchScreen;
            hal::OutputPin yPlus;
            hal::OutputPin yMinus;
            infra::ProxyCreator<AnalogToDigitalPin, void()> xPlusAnalogPin;
            infra::TimerSingleShot startConversion;
            PixelPosition pixelPosition;
        };

        class StateYMeasurement
            : public State
        {
        public:
            explicit StateYMeasurement(TouchScreen& touchScreen);

        private:
            void OnMeasurementDone(uint32_t pixelPosition);

        private:
            TouchScreen& touchScreen;
            hal::OutputPin xPlus;
            hal::OutputPin xMinus;
            infra::ProxyCreator<AnalogToDigitalPin, void()> yPlusAnalogPin;
            infra::TimerSingleShot startConversion;
            PixelPosition pixelPosition;
        };

    private:
        hal::GpioPin& xPlus;
        hal::GpioPin& xMinus;
        hal::GpioPin& yPlus;
        hal::GpioPin& yMinus;
        infra::CreatorBase<AnalogToDigitalPin, void()>& xPlusAnalogPin;
        infra::CreatorBase<AnalogToDigitalPin, void()>& yPlusAnalogPin;
        uint32_t touchDelta;

        infra::AutoResetFunction<void(std::optional<infra::Point> position)> onTouched;
        infra::PolymorphicVariant<State, StateIdle, StateTouchMeasurement, StateXMeasurement, StateYMeasurement> state;

        uint32_t xResult;
        uint32_t yResult;
    };
}

#endif
