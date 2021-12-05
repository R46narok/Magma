//
// Created by Acer on 9.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Window.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

namespace Magma
{
    Surface::Surface()
        : _Surface(VK_NULL_HANDLE)
    {
        auto instance = Graphics::GetInstance()->GetVulkanInstance();
        auto window = Window::Get();
        Graphics::CheckVk(glfwCreateWindowSurface(instance, window->GetWindow(), nullptr, &_Surface));
    }

    Surface::~Surface() noexcept
    {
        auto instance = Graphics::GetInstance()->GetVulkanInstance();
        vkDestroySurfaceKHR(instance, _Surface, nullptr);
    }
}