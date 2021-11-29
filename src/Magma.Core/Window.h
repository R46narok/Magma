//
// Created by Acer on 7.11.2021 г..
//

#ifndef MAGMA_WINDOW_H
#define MAGMA_WINDOW_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Engine/Module.h"

#include <string>
#include <functional>

using GLFWwindow = struct GLFWwindow;

namespace Magma
{
    struct WindowDescriptor
    {
        int iWidth;
        int iHeight;
        bool Vsync;
        std::string Title;
        std::function<void(void*)> EventCallback;
    };


    class _Magma_Dll Window : public Module::Registrar<Window>
    {
    private:
        inline static const bool Registered = Register(Stage::Pre);
    public:
        Window();
        explicit Window(const WindowDescriptor& descriptor);
        ~Window() noexcept;

        void Update() override;

        bool IsAlive() const noexcept;

        [[nodiscard]] const GLFWwindow * GetWindow() const noexcept { return _pRawWindow; }
        [[nodiscard]] GLFWwindow * GetWindow() noexcept { return _pRawWindow; }
    private:
        struct UserPtr
        {
            int iWidth;
            int iHeight;
            std::function<void(void*)> EventCallback;
        };

        GLFWwindow* _pRawWindow;
        std::string _Title;
        UserPtr _UserPtr;
        bool _Vsync;
    };
}

#endif //MAGMA_WINDOW_H
