#include "preview/touch/TouchScreenToTouchRecipientInteractor.hpp"

namespace services
{
    TouchScreenToTouchRecipientInteractor::TouchScreenToTouchRecipientInteractor(services::TouchScreen& touchScreen, services::TouchRecipient& touchRecipient, infra::Vector displaySize, infra::Vector touchOffset)
        : touchScreen(touchScreen)
        , touchRecipient(touchRecipient)
        , displaySize(displaySize)
        , touchOffset(touchOffset)
        , measureTimer(std::chrono::milliseconds(25), [this]()
              {
                  MeasureTouch();
              })
    {}

    void TouchScreenToTouchRecipientInteractor::MeasureTouch()
    {
        if (!measurementBusy)
        {
            measurementBusy = true;
            touchScreen.Measure([this](std::optional<infra::Point> position)
                {
                    if (position)
                    {
                        infra::Point viewPosition = infra::Point(-position->x, position->y) + infra::DeltaX(displaySize.deltaX) + touchOffset;
                        HandleTouch(viewPosition);
                    }
                    else
                        HandleNoTouch();

                    measurementBusy = false;
                });
        }
    }

    void TouchScreenToTouchRecipientInteractor::HandleTouch(infra::Point position)
    {
        successiveNoTouches = 0;

        if (lastTouches.full())
            lastTouches.pop_front();
        lastTouches.push_back(position);

        if (!touching)
        {
            touching = true;
            touchRecipient.StartTouch(position);
        }
        else
            touchRecipient.DragTo(position);
    }

    void TouchScreenToTouchRecipientInteractor::HandleNoTouch()
    {
        if (touching)
        {
            if (lastTouches.full() && infra::ManhattanDistance(lastTouches[0], lastTouches[1]) > 5 && infra::ManhattanDistance(lastTouches[1], lastTouches[2]) > 5)
            {
                int16_t xDiff = lastTouches[2].x - lastTouches[1].x;
                int16_t yDiff = lastTouches[2].y - lastTouches[1].y;

                if (std::abs(xDiff) > std::abs(yDiff))
                {
                    if (xDiff > 0)
                        touchRecipient.Swipe(services::Direction::right);
                    else
                        touchRecipient.Swipe(services::Direction::left);
                }
                else
                {
                    if (yDiff > 0)
                        touchRecipient.Swipe(services::Direction::down);
                    else
                        touchRecipient.Swipe(services::Direction::up);
                }

                touching = false;
                lastTouches.clear();
            }
            else
            {
                ++successiveNoTouches;
                if (successiveNoTouches > 4)
                {
                    touching = false;
                    touchRecipient.StopTouch();
                    lastTouches.clear();
                }
            }
        }
    }
}
