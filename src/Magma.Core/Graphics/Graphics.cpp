//
// Created by Acer on 12.11.2021 г..
//

#include <Magma.Core/Engine/Log.h>
#include "Magma.Core/Graphics/Graphics.h"

#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


namespace Magma
{
    Graphics::Graphics()
    {
        moduleInstance = this;
        _Instance = CreateRef<Instance>();
        _Surface = CreateRef<Surface>();
        _PhysicalDevice = CreateRef<PhysicalDevice>();
        _LogicalDevice = CreateRef<LogicalDevice>();
        _Swapchain = CreateRef<Swapchain>();
        _Pool = CreateRef<CommandPool>(_PhysicalDevice, _LogicalDevice, _Surface);
        _Renderer = CreateScope<Renderer2D>();
    }

    Graphics::~Graphics() noexcept
    {
        _Renderer.reset();
        _Pool.reset();
        _Swapchain.reset();
        _LogicalDevice.reset();
        _Surface.reset();
        _Instance.reset();
    }

    uint32_t currentFrame = 0;

    void Graphics::Update()
    {
        _Renderer->Update();
    }

    void Graphics::UpdateUniformBuffer(uint32_t currentImage)
    {
    }

    void Graphics::CheckVk(VkResult result)
    {
        if (result != VK_SUCCESS)
            __debugbreak();
    }
}


