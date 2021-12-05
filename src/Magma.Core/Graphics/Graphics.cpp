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

        if (!glslang::InitializeProcess())
            throw std::runtime_error("Failed to initialize glslang process");

        _Instance = CreateRef<Instance>();
        _Surface = CreateRef<Surface>();
        _PhysicalDevice = CreateRef<PhysicalDevice>(_Instance);
        _LogicalDevice = CreateRef<LogicalDevice>(_PhysicalDevice, _Surface);
        _Swapchain = CreateRef<Swapchain>();
        _Pool = CreateRef<CommandPool>(_PhysicalDevice, _LogicalDevice, _Surface);
        _Renderer = CreateScope<Renderer2D>();
    }

    Graphics::~Graphics() noexcept
    {
        glslang::FinalizeProcess();

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

    void Graphics::CheckVk(VkResult result)
    {
        if (result >= 0) return;

        auto failure = StringifyResultVk(result);
        _Magma_Core_Error(failure);
        __debugbreak();
    }

    std::string Graphics::StringifyResultVk(VkResult result)
    {
        switch (result)
        {
            case VK_SUCCESS: return "Success";
            case VK_NOT_READY: return "A fence or query has not yet completed";
            case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
            case VK_EVENT_SET: return "An event is signaled";
            case VK_EVENT_RESET: return "An event is unsignaled";
            case VK_INCOMPLETE: return "A return array was too small for the result";
            case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
            case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
            case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
            case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
            case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
            case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
            case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
            case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
            case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
            case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
            case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
            case VK_ERROR_OUT_OF_POOL_MEMORY: return "A allocation failed due to having no more space in the descriptor pool";
            case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
            case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
            case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";
            default: return "Unknown Vulkan error";
        }
    }
}


