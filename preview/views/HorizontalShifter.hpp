#ifndef PREVIEW_HORIZONTAL_SHIFTER_HPP
#define PREVIEW_HORIZONTAL_SHIFTER_HPP

#include "preview/interfaces/View.hpp"

namespace services
{
    class HorizontalShifter
        : public View
    {
    public:
        template<class SubView>
        class WithView;

        explicit HorizontalShifter(View& subView);

        void SetShift(int16_t newShift);

        // Implementation of View
        infra::Vector MinimumSize() const override;
        infra::Vector MaximumSize() const override;
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        void ResetSize() override;

    protected:
        void ViewRegionChanged() override;

    private:
        View& subView;
        int16_t shift = 0;
    };

    template<class Member>
    class BaseFromMember
    {
    public:
        template<class... Args>
        BaseFromMember(Args&&... args)
            : member(std::forward<Args>(args)...)
        {}

        Member member;
    };

    template<class SubView>
    class HorizontalShifter::WithView
        : BaseFromMember<SubView>
        , public HorizontalShifter
    {
    public:
        template<class... Args>
        WithView(Args&&... args)
            : BaseFromMember<SubView>(std::forward<Args>(args)...)
            , HorizontalShifter(this->member)
        {}

        SubView& GetView()
        {
            return this->member;
        }
    };
}

#endif
