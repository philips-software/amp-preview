#ifndef PREVIEW_VIEW_SCROLLING_HPP
#define PREVIEW_VIEW_SCROLLING_HPP

#include "infra/timer/Timer.hpp"
#include "preview/interfaces/View.hpp"

namespace services
{
    class ViewScrolling
        : public View
    {
    public:
        template<class T>
        class WithView;

        ViewScrolling(View& subView, infra::Duration pixelSpeed, uint32_t pixelsPerStep, infra::Duration hold);

        virtual infra::Vector MinimumSize() const override;
        virtual infra::Vector MaximumSize() const override;
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

    protected:
        virtual void ViewRegionChanged() override;

    private:
        void FirstStep();
        void Step();
        void SetOffset(uint16_t newOffset);

    private:
        services::View& subView;
        infra::Duration pixelSpeed;
        infra::Duration hold;
        uint16_t pixelsPerStep;

        uint16_t offset = 0;
        infra::TimerRepeating timer{ hold, [this]()
            {
                FirstStep();
            } };
    };

    template<class T>
    class ViewScrolling::WithView
    {
    public:
        template<class... Args>
        WithView(infra::Duration pixelSpeed, uint32_t pixelsPerStep, infra::Duration hold, Args&&... args)
            : subView(std::forward<Args>(args)...)
            , viewScrolling(subView, pixelSpeed, pixelsPerStep, hold)
        {}

        T& SubView()
        {
            return subView;
        }

        ViewScrolling& Scrolling()
        {
            return viewScrolling;
        }

    private:
        T subView;
        ViewScrolling viewScrolling;
    };
}

#endif
