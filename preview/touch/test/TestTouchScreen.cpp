#include "hal/interfaces/test_doubles/GpioMock.hpp"
#include "infra/timer/test_helper/ClockFixture.hpp"
#include "infra/util/test_helper/MockCallback.hpp"
#include "preview/touch/TouchScreen.hpp"
#include "gmock/gmock.h"

class AnalogToDigitalPinMock
    : public services::TouchScreen::AnalogToDigitalPin
{
public:
    MOCK_METHOD1(Measure, void(const infra::Function<void(services::TouchScreen::PixelPosition pixelPosition)>& onDone));
};

class TouchScreenTest
    : public testing::Test
    , public infra::ClockFixture
{
public:
    TouchScreenTest()
        : touchScreen(xPlus, xMinus, yPlus, yMinus, xPlusAnalogPin, yPlusAnalogPin, 200)
    {}

    void DoTouchMeasurement()
    {
        EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

        touchScreen.Measure(onResult);

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        onMeasurementDone(services::TouchScreen::PixelPosition(xTouchResult));

        EXPECT_CALL(yMinus, ResetConfig());
        EXPECT_CALL(xMinus, ResetConfig());
    }

    void DoXMeasurement()
    {
        EXPECT_CALL(yPlus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone(services::TouchScreen::PixelPosition(yTouchResult));

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(yMinus, ResetConfig());
        EXPECT_CALL(yPlus, ResetConfig());
    }

    void DoYMeasurement()
    {
        EXPECT_CALL(xPlus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone(services::TouchScreen::PixelPosition(xResult));

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(xMinus, ResetConfig());
        EXPECT_CALL(xPlus, ResetConfig());
    }

    void DoFinalTouchMeasurement()
    {
        EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone(services::TouchScreen::PixelPosition(yResult));

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_)).WillOnce(testing::SaveArg<0>(&onMeasurementDone));
        onMeasurementDone(services::TouchScreen::PixelPosition(xTouchResult));

        EXPECT_CALL(yMinus, ResetConfig());
        EXPECT_CALL(xMinus, ResetConfig());
    }

public:
    testing::StrictMock<hal::GpioPinMock> xPlus;
    testing::StrictMock<hal::GpioPinMock> xMinus;
    testing::StrictMock<hal::GpioPinMock> yPlus;
    testing::StrictMock<hal::GpioPinMock> yMinus;
    infra::Creator<services::TouchScreen::AnalogToDigitalPin, AnalogToDigitalPinMock, void()> xPlusAnalogPin;
    infra::Creator<services::TouchScreen::AnalogToDigitalPin, AnalogToDigitalPinMock, void()> yPlusAnalogPin;
    services::TouchScreen touchScreen;

    uint32_t xResult = 0;
    uint32_t yResult = 0;
    uint32_t xTouchResult = 0;
    uint32_t yTouchResult = 0;

    infra::Function<void(services::TouchScreen::PixelPosition pixelPosition)> onMeasurementDone;
    infra::Function<void(infra::Optional<infra::Point> position)> onResult = [](infra::Optional<infra::Point> position) {};
};

TEST_F(TouchScreenTest, Measure_results_in_touch_measurement)
{
    EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, true));
    EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

    touchScreen.Measure(onResult);

    EXPECT_CALL(yMinus, ResetConfig());
    EXPECT_CALL(xMinus, ResetConfig());
}

TEST_F(TouchScreenTest, on_no_touch_none_is_reported)
{
    infra::VerifyingFunctionMock<void(infra::Optional<infra::Point>)> expectOnResult(infra::none);
    onResult = expectOnResult;

    xTouchResult = 400;
    yTouchResult = 100;

    DoTouchMeasurement();

    onMeasurementDone(services::TouchScreen::PixelPosition(yTouchResult));
}

TEST_F(TouchScreenTest, after_touch_measurement_is_done_x_measurement_starts)
{
    DoTouchMeasurement();

    EXPECT_CALL(yPlus, Config(hal::PinConfigType::output, true));
    EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

    onMeasurementDone(services::TouchScreen::PixelPosition(yTouchResult));

    EXPECT_CALL(yMinus, ResetConfig());
    EXPECT_CALL(yPlus, ResetConfig());
}

TEST_F(TouchScreenTest, after_measurement_is_done_y_measurement_starts)
{
    DoTouchMeasurement();
    DoXMeasurement();

    EXPECT_CALL(xPlus, Config(hal::PinConfigType::output, true));
    EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, false));

    onMeasurementDone(services::TouchScreen::PixelPosition(xResult));

    EXPECT_CALL(xMinus, ResetConfig());
    EXPECT_CALL(xPlus, ResetConfig());
}

TEST_F(TouchScreenTest, after_last_measurement_result_is_reported)
{
    infra::VerifyingFunctionMock<void(infra::Optional<infra::Point>)> expectOnResult(infra::MakeOptional(infra::Point()));
    onResult = expectOnResult;

    DoTouchMeasurement();
    DoXMeasurement();
    DoYMeasurement();
    DoFinalTouchMeasurement();

    onMeasurementDone(services::TouchScreen::PixelPosition(yTouchResult));
}

TEST_F(TouchScreenTest, on_touch_position_is_reported)
{
    infra::VerifyingFunctionMock<void(infra::Optional<infra::Point>)> expectOnResult(infra::MakeOptional(infra::Point(42, 134)));
    onResult = expectOnResult;

    xResult = 42;
    yResult = 134;
    xTouchResult = 190;
    yTouchResult = 100;

    DoTouchMeasurement();
    DoXMeasurement();
    DoYMeasurement();
    DoFinalTouchMeasurement();

    onMeasurementDone(services::TouchScreen::PixelPosition(yTouchResult));
}
