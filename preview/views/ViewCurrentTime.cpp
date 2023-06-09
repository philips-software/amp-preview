#include "preview/views/ViewCurrentTime.hpp"
#include "infra/stream/StringOutputStream.hpp"
#include "infra/timer/PartitionedTime.hpp"

namespace services
{
    ViewCurrentTime::ViewCurrentTime(uint32_t timerServiceId, TextAttributes attributes)
        : ViewText(attributes)
        , timer([this]()
              {
                  Update();
              },
              timerServiceId)
    {}

    void ViewCurrentTime::Update()
    {
        time.clear();
        auto now = infra::PartitionedTime(timer.Now());
        infra::StringOutputStream stream(time);
        stream << infra::Width(2, '0') << now.hours << infra::resetWidth << ":" << infra::Width(2, '0') << now.minutes;
        SetText(time);
    }

    ViewCurrentTime::TimerEachMinute::TimerEachMinute(const infra::Function<void()>& action, uint32_t timerServiceId)
        : Timer(timerServiceId)
    {
        SetNextTrigger(Now(), action);
        Action()();
    }

    void ViewCurrentTime::TimerEachMinute::ComputeNextTriggerTime()
    {
        SetNextTrigger(Now(), Action());
    }

    void ViewCurrentTime::TimerEachMinute::Jumped(infra::TimePoint from, infra::TimePoint to)
    {
        if (RoundedUp(from) != RoundedUp(to))
        {
            Action()();
            SetNextTrigger(to, Action());
        }
    }

    void ViewCurrentTime::TimerEachMinute::SetNextTrigger(infra::TimePoint time, const infra::Function<void()>& action)
    {
        SetNextTriggerTime(RoundedUp(time), action);
    }

    infra::TimePoint ViewCurrentTime::TimerEachMinute::RoundedUp(infra::TimePoint time) const
    {
        auto rounded = infra::PartitionedTime(time + std::chrono::minutes(1));
        rounded.seconds = 0;
        return rounded.ToTimePoint();
    }
}
