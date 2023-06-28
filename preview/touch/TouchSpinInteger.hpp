#ifndef PREVIEW_TOUCH_SPIN_INTEGER_HPP
#define PREVIEW_TOUCH_SPIN_INTEGER_HPP

#include "preview/touch/TouchRecipient.hpp"
#include "preview/views/ViewFadingText.hpp"

namespace services
{
    class TouchSpinInteger
        : public TouchRecipient
    {
    public:
        class WithViewText;
        class WithViewFadingText;

        template<class ViewType>
        class WithViewTextDescendant;
        template<class ViewType>
        class WithViewFadingTextDescendant;

        TouchSpinInteger(View& view, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement);
        TouchSpinInteger(View& view, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width);

        // Implementation of TouchRecipient
        void StartTouch(infra::Point point) override;
        void DragIn(infra::Point point) override;
        void DragTo(infra::Point point) override;
        void DragOut() override;
        void StopTouch() override;
        void Swipe(Direction direction) override;
        View& GetView() override;

        int32_t Value() const;
        void SetValue(int32_t newValue);
        void SetValue(int32_t newValue, Direction from);
        void SetInitialValue();

    protected:
        virtual void Report(int32_t steps, Direction from);

        virtual void SetText(infra::BoundedConstString text) = 0;
        virtual void SetText(infra::BoundedConstString text, Direction from) = 0;

    private:
        void WriteValueString();
        int32_t AdjustedValue(int32_t newValue) const;
        int32_t Modulus(int32_t a, int32_t b) const;

    private:
        View& view;
        int32_t value;
        int32_t from;
        int32_t to;
        bool circular;
        uint16_t distancePerIncrement;
        infra::Optional<uint8_t> width;

        infra::Optional<infra::Point> startTouch;
        int32_t stepsReported = 0;

        infra::BoundedString::WithStorage<12> valueString;
    };

    class TouchSpinInteger::WithViewText
        : public ViewText
        , public TouchSpinInteger
    {
    public:
        WithViewText(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, TextAttributes attributes);
        WithViewText(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, TextAttributes attributes);

        void SetText(infra::BoundedConstString text) override;
        void SetText(infra::BoundedConstString text, Direction from) override;
    };

    class TouchSpinInteger::WithViewFadingText
        : public ViewFadingText
        , public TouchSpinInteger
    {
    public:
        template<std::size_t Size>
        using WithStorage = infra::WithStorage<infra::WithStorage<TouchSpinInteger::WithViewFadingText, infra::BoundedString::WithStorage<Size>>, infra::BoundedString::WithStorage<Size>>;

        WithViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, FadingTextAttributes attributes);
        WithViewFadingText(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, FadingTextAttributes attributes);

        void SetText(infra::BoundedConstString text) override;
        void SetText(infra::BoundedConstString text, Direction from) override;
    };

    template<class ViewType>
    class TouchSpinInteger::WithViewTextDescendant
        : public ViewType
        , public TouchSpinInteger
    {
    public:
        template<class... Args>
        WithViewTextDescendant(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, Args&&... args);
        template<class... Args>
        WithViewTextDescendant(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, Args&&... args);

        ViewType& GetView() override;
        void SetText(infra::BoundedConstString text) override;
        void SetText(infra::BoundedConstString text, Direction from) override;
    };

    template<class ViewType>
    class TouchSpinInteger::WithViewFadingTextDescendant
        : public ViewType
        , public TouchSpinInteger
    {
    public:
        template<std::size_t Size>
        using WithStorage = infra::WithStorage<infra::WithStorage<TouchSpinInteger::WithViewFadingTextDescendant<TheView>, infra::BoundedString::WithStorage<Size>>, infra::BoundedString::WithStorage<Size>>;

        template<class... Args>
        WithViewFadingTextDescendant(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, Args&&... args);
        template<class... Args>
        WithViewFadingTextDescendant(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, Args&&... args);

        ViewType& GetView() override;
        void SetText(infra::BoundedConstString text) override;
        void SetText(infra::BoundedConstString text, Direction from) override;
    };

    ////    Implementation    ////

    template<class TheView>
    template<class... Args>
    TouchSpinInteger::WithViewTextDescendant<TheView>::WithViewTextDescendant(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, Args&&... args)
        : TheView(args...)
        , TouchSpinInteger(*this, start, from, to, distancePerIncrement)
    {
        SetInitialValue();
        this->SetTextAndResize(this->Text());
    }

    template<class TheView>
    template<class... Args>
    TouchSpinInteger::WithViewTextDescendant<TheView>::WithViewTextDescendant(int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, Args&&... args)
        : TheView(args...)
        , TouchSpinInteger(*this, start, from, to, distancePerIncrement, width)
    {
        SetInitialValue();
        this->SetTextAndResize(this->Text());
    }

    template<class TheView>
    TheView& TouchSpinInteger::WithViewTextDescendant<TheView>::GetView()
    {
        return *this;
    }

    template<class TheView>
    void TouchSpinInteger::WithViewTextDescendant<TheView>::SetText(infra::BoundedConstString text)
    {
        TheView::SetText(valueString);
    }

    template<class TheView>
    void TouchSpinInteger::WithViewTextDescendant<TheView>::SetText(infra::BoundedConstString text, Direction from)
    {
        TheView::SetText(valueString);
    }

    template<class TheView>
    template<class... Args>
    TouchSpinInteger::WithViewFadingTextDescendant<TheView>::WithViewFadingTextDescendant(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, Args&&... args)
        : TheView(buffer1, buffer2, args...)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement)
    {
        SetInitialValue();
        this->SetTextAndResize(this->Text());
    }

    template<class TheView>
    template<class... Args>
    TouchSpinInteger::WithViewFadingTextDescendant<TheView>::WithViewFadingTextDescendant(infra::BoundedString& buffer1, infra::BoundedString& buffer2, int32_t start, int32_t from, int32_t to, bool circular, uint16_t distancePerIncrement, uint8_t width, Args&&... args)
        : TheView(buffer1, buffer2, args...)
        , TouchSpinInteger(*this, start, from, to, circular, distancePerIncrement, width)
    {
        SetInitialValue();
        this->SetTextAndResize(this->Text());
    }

    template<class TheView>
    TheView& TouchSpinInteger::WithViewFadingTextDescendant<TheView>::GetView()
    {
        return *this;
    }

    template<class TheView>
    void TouchSpinInteger::WithViewFadingTextDescendant<TheView>::SetText(infra::BoundedConstString text)
    {
        TheView::SetText(valueString);
    }

    template<class TheView>
    void TouchSpinInteger::WithViewFadingTextDescendant<TheView>::SetText(infra::BoundedConstString text, Direction from)
    {
        TheView::FadeIn(valueString, from);
    }
}

#endif
