//
// Created by Acer on 7.11.2021 г..
//

#include "Magma.Core/Devices/Instance.h"
#include "Magma.Core/Devices/Ext.h"
#include "Magma.Core/Debug/ValidationLayers.h"
#include "Magma.Core/Engine/Log.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    constexpr const char* AppName = "Magma";
    constexpr const char* EngineName = "Magma Engine";

    Instance::Instance()
        : _Instance(VK_NULL_HANDLE)
    {
        VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = AppName,
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = EngineName,
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        auto extensions = RequiredGraphicsExtensions();
        auto layers = QueryValidationLayers();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        PopulateDebugMessengerCreateInfo(debugCreateInfo);

        VkInstanceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo,
            .pApplicationInfo = &appInfo
        };

        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());

        auto result = vkCreateInstance(&createInfo, nullptr, &_Instance);
        _Magma_Assert(result == VK_SUCCESS, _Magma_Core_Error("Failed to create a VkInstance"));
        _Magma_Core_Info("VkInstance created");

        EnableValidationLayersOutput(_Instance);
    }

    Instance::~Instance() noexcept
    {
        DestroyDebugUtilsMessenger(_Instance);
        vkDestroyInstance(_Instance, nullptr);
    }
}