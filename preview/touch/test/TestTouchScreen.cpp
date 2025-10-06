#include "hal/interfaces/test_doubles/GpioMock.hpp"
#include "infra/timer/test_helper/ClockFixture.hpp"
#include "infra/util/test_helper/MockCallback.hpp"
#include "preview/touch/TouchScreen.hpp"
#include "gmock/gmock.h"

class AnalogToDigitalPinMock
    : public services::TouchScreen::AnalogToDigitalPin
{
public:
    MOCK_METHOD2(Measure, void(services::TouchScreen::AnalogToDigitalPin::SamplesRange samples, const infra::Function<void()>& onDone));
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

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = xTouchResult;
                onMeasurementDone = onDone;
            });
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = yTouchResult;
                onMeasurementDone = onDone;
            });
        onMeasurementDone();

        EXPECT_CALL(yMinus, ResetConfig());
        EXPECT_CALL(xMinus, ResetConfig());
    }

    void DoXMeasurement()
    {
        EXPECT_CALL(yPlus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone();

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = xResult;
                onMeasurementDone = onDone;
            });
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(yMinus, ResetConfig());
        EXPECT_CALL(yPlus, ResetConfig());
    }

    void DoYMeasurement()
    {
        EXPECT_CALL(xPlus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone();

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = yResult;
                onMeasurementDone = onDone;
            });
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(xMinus, ResetConfig());
        EXPECT_CALL(xPlus, ResetConfig());
    }

    void DoFinalTouchMeasurement()
    {
        EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, true));
        EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

        onMeasurementDone();

        EXPECT_CALL(*xPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = yResult;
                onMeasurementDone = onDone;
            });
        ForwardTime(std::chrono::microseconds(100));

        EXPECT_CALL(*yPlusAnalogPin, Measure(testing::_, testing::_)).WillOnce([this](auto samples, const auto& onDone)
            {
                samples.front() = xTouchResult;
                onMeasurementDone = onDone;
            });
        onMeasurementDone();

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

    services::TouchScreen::PixelPosition xResult{ 0 };
    services::TouchScreen::PixelPosition yResult{ 0 };
    services::TouchScreen::PixelPosition xTouchResult{ 0 };
    services::TouchScreen::PixelPosition yTouchResult{ 0 };

    infra::Function<void()> onMeasurementDone;
    infra::Function<void(std::optional<infra::Point> position)> onResult = [](std::optional<infra::Point> position) {};
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
    infra::VerifyingFunction<void(std::optional<infra::Point>)> expectOnResult(std::nullopt);
    onResult = expectOnResult;

    xTouchResult = services::TouchScreen::PixelPosition{ 400 };
    yTouchResult = services::TouchScreen::PixelPosition{ 100 };

    DoTouchMeasurement();

    onMeasurementDone();
}

TEST_F(TouchScreenTest, after_touch_measurement_is_done_x_measurement_starts)
{
    DoTouchMeasurement();

    EXPECT_CALL(yPlus, Config(hal::PinConfigType::output, true));
    EXPECT_CALL(yMinus, Config(hal::PinConfigType::output, false));

    onMeasurementDone();

    EXPECT_CALL(yMinus, ResetConfig());
    EXPECT_CALL(yPlus, ResetConfig());
}

TEST_F(TouchScreenTest, after_measurement_is_done_y_measurement_starts)
{
    DoTouchMeasurement();
    DoXMeasurement();

    EXPECT_CALL(xPlus, Config(hal::PinConfigType::output, true));
    EXPECT_CALL(xMinus, Config(hal::PinConfigType::output, false));

    onMeasurementDone();

    EXPECT_CALL(xMinus, ResetConfig());
    EXPECT_CALL(xPlus, ResetConfig());
}

TEST_F(TouchScreenTest, after_last_measurement_result_is_reported)
{
    infra::VerifyingFunction<void(std::optional<infra::Point>)> expectOnResult(std::make_optional(infra::Point()));
    onResult = expectOnResult;

    DoTouchMeasurement();
    DoXMeasurement();
    DoYMeasurement();
    DoFinalTouchMeasurement();

    onMeasurementDone();
}

TEST_F(TouchScreenTest, on_touch_position_is_reported)
{
    infra::VerifyingFunction<void(std::optional<infra::Point>)> expectOnResult(std::make_optional(infra::Point(42, 134)));
    onResult = expectOnResult;

    xResult = services::TouchScreen::PixelPosition{ 42 };
    yResult = services::TouchScreen::PixelPosition{ 134 };
    xTouchResult = services::TouchScreen::PixelPosition{ 190 };
    yTouchResult = services::TouchScreen::PixelPosition{ 100 };

    DoTouchMeasurement();
    DoXMeasurement();
    DoYMeasurement();
    DoFinalTouchMeasurement();

    onMeasurementDone();
}
