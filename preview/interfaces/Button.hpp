#ifndef PREVIEW_BUTTON_HPP
#define PREVIEW_BUTTON_HPP

namespace services
{
    class Button
    {
    protected:
        Button() = default;
        Button(const Button& other) = delete;
        Button& operator=(const Button& other) = delete;
        ~Button() = default;

    public:
        virtual void Activate() = 0;
        virtual void Deactivate() = 0;
    };
}

#endif
