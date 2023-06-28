#ifndef PREVIEW_VIEW_CURRENT_TIME_HPP
#define PREVIEW_VIEW_CURRENT_TIME_HPP

#include "infra/timer/Timer.hpp"
#include "preview/views/ViewText.hpp"

namespace services
{
    class ViewCurrentTime
        : public ViewText
    {
    public:
        ViewCurrentTime(uint32_t timerServiceId, TextAttributes attributes);

    private:
        void Update();

    private:
        class TimerEachMinute
            : public infra::Timer
        {
        public:
            TimerEachMinute(const infra::Function<void()>& action, uint32_t timerServiceId);

            // Implementation of infra::Timer
            void ComputeNextTriggerTime() override;
            void Jumped(infra::TimePoint from, infra::TimePoint to) override;

        private:
            void SetNextTrigger(infra::TimePoint time, const infra::Function<void()>& action);
            infra::TimePoint RoundedUp(infra::TimePoint time) const;
        };

    private:
        infra::BoundedString::WithStorage<6> time;
        TimerEachMinute timer;
    };
}

#endif
