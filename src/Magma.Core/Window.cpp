//
// Created by Acer on 7.11.2021 г..
//

#include "Magma.Core/Window.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Magma
{
    Window::Window()
            : _Title("Smart City"),
              _UserPtr({.iWidth = 1280, .iHeight = 640}),
              _Vsync(false)
    {
        _Magma_Assert(glfwInit(), std::cout << "Could not init glfw\n");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _pRawWindow = glfwCreateWindow(_UserPtr.iWidth, _UserPtr.iHeight, _Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(_pRawWindow, &_UserPtr);
    }

    Window::~Window() noexcept
    {
        glfwDestroyWindow(_pRawWindow);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwPollEvents();
    }

    bool Window::IsAlive() const noexcept {
        return! glfwWindowShouldClose(_pRawWindow);
    }

}