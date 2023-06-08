#include "preview/touch/TouchFt5x06.hpp"

namespace services
{
    namespace
    {
        const hal::I2cAddress touchAddress(0x70 >> 1);

        const uint8_t registerStatus = 2;
        const uint8_t registerXHigh = 3;
    }

    TouchFt5x06::TouchFt5x06(hal::I2cMaster& i2c, hal::GpioPin& interrupt)
        : i2c(i2c)
        , interrupt(interrupt)
    {
        interrupt.EnableInterrupt([this]()
            {
                if (idle)
                    Measure();
            },
            hal::InterruptTrigger::fallingEdge);
    }

    void TouchFt5x06::Measure()
    {
        idle = false;

        i2c.SendData(touchAddress, infra::MakeByteRange(registerStatus), hal::Action::repeatedStart, [this](hal::Result result, uint32_t numberOfBytesSent)
            {
                i2c.ReceiveData(touchAddress, infra::MakeByteRange(status), hal::Action::stop, [this](hal::Result result)
                    {
                        if (status != 0)
                            MeasureTouch();
                        else
                        {
                            NoTouch();
                            idle = true;
                        }
                    });
            });
    }

    void TouchFt5x06::MeasureTouch()
    {
        i2c.SendData(touchAddress, infra::MakeByteRange(registerXHigh), hal::Action::repeatedStart, [this](hal::Result result, uint32_t numberOfBytesSent)
            {
                i2c.ReceiveData(touchAddress, infra::MakeByteRange(xy), hal::Action::stop, [this](hal::Result result)
                    {
                        Touched(infra::Point(((static_cast<uint8_t>(xy) & 0xf) << 8) + static_cast<uint8_t>(xy >> 8), (static_cast<uint8_t>(xy >> 16) << 8) + static_cast<uint8_t>(xy >> 24)));
                        idle = true;
                        Measure();
                    });
            });
    }

    TouchFt5x06OnTouchRecipient::TouchFt5x06OnTouchRecipient(hal::I2cMaster& i2c, hal::GpioPin& interrupt, services::TouchRecipient& touchRecipient)
        : TouchFt5x06(i2c, interrupt)
        , touchRecipient(touchRecipient)
    {}

    void TouchFt5x06OnTouchRecipient::Touched(infra::Point position)
    {
        position = infra::Point(position.y, position.x);

        if (touching)
            touchRecipient.DragTo(position);
        else
            touchRecipient.StartTouch(position);

        touching = true;
    }

    void TouchFt5x06OnTouchRecipient::NoTouch()
    {
        if (touching)
            touchRecipient.StopTouch();

        touching = false;
    }
}
