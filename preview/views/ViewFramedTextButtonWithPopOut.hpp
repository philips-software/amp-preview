#ifndef PREVIEW_VIEW_FRAMED_TEXT_BUTTON_WITH_POP_OUT_HPP
#define PREVIEW_VIEW_FRAMED_TEXT_BUTTON_WITH_POP_OUT_HPP

#include "preview/views/ViewFramedTextButton.hpp"
#include "preview/views/ViewOffsetRegion.hpp"
#include <optional>

namespace services
{
    class ViewFramedTextButtonWithPopOut
        : public ViewFramedTextButton
    {
    public:
        ViewFramedTextButtonWithPopOut(FramedTextButtonAttributes attributes, infra::BoundedConstString text, infra::RegionOffset regionOffset);

        // Implementation of Button
        void Activate() override;
        void Deactivate() override;

        // Implementation of View
        void Paint(hal::Canvas& canvas, infra::Region boundingRegion) override;
        void ResetSize() override;

    protected:
        void ViewRegionChanged() override;

    private:
        infra::RegionOffset regionOffset;
        std::optional<ViewOffsetRegion::WithView<ViewFramedText>> popOut;
    };
}

#endif
