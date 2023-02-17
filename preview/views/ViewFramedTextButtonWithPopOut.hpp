#ifndef PREVIEW_VIEW_FRAMED_TEXT_BUTTON_WITH_POP_OUT_HPP
#define PREVIEW_VIEW_FRAMED_TEXT_BUTTON_WITH_POP_OUT_HPP

#include "infra/util/Optional.hpp"
#include "preview/views/ViewFramedTextButton.hpp"
#include "preview/views/ViewOffsetRegion.hpp"

namespace services
{
    class ViewFramedTextButtonWithPopOut
        : public ViewFramedTextButton
    {
    public:
        ViewFramedTextButtonWithPopOut(FramedTextButtonAttributes attributes, infra::BoundedConstString text, infra::RegionOffset regionOffset);

        // Implementation of Button
        virtual void Activate() override;
        virtual void Deactivate() override;

        // Implementation of View
        virtual void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        virtual void ResetSize() override;

    protected:
        virtual void ViewRegionChanged() override;

    private:
        infra::RegionOffset regionOffset;
        infra::Optional<ViewOffsetRegion::WithView<ViewFramedText>> popOut;
    };
}

#endif
