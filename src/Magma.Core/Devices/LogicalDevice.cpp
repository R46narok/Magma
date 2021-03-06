//
// Created by Acer on 9.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Devices/LogicalDevice.h"
#include "Magma.Core/Debug/ValidationLayers.h"
#include "Magma.Core/Graphics/Graphics.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <array>

namespace Magma
{
    LogicalDevice::LogicalDevice(const Ref<PhysicalDevice>& physicalDevice, const Ref<Surface>& surface)
        : _Device(VK_NULL_HANDLE), _GraphicsQueue(VK_NULL_HANDLE), _PresentQueue(VK_NULL_HANDLE)
    {
        auto indices = FindQueueFamilies(physicalDevice->GetVulkanPhysicalDevice(), surface->GetVulkanSurface());
        std::array<VkDeviceQueueCreateInfo, 2> queueCreateInfos{};
        std::array<uint32_t , 2> queueFamilies{indices.GraphicsFamily.value(), indices.PresentFamily.value()};

        for(int i = 0; i < queueCreateInfos.size(); ++i)
            PopulateDeviceQueueCreateInfo(queueCreateInfos[i], queueFamilies[i]);

        VkPhysicalDeviceFeatures deviceFeatures{};

        auto layers = QueryValidationLayers();
        VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = RequiredDeviceExtensions.size(),
            .ppEnabledExtensionNames = RequiredDeviceExtensions.data(),
            .pEnabledFeatures = &deviceFeatures,
        };

        Graphics::CheckVk(vkCreateDevice(physicalDevice->GetVulkanPhysicalDevice(), &deviceCreateInfo, nullptr, &_Device));

        vkGetDeviceQueue(_Device, indices.GraphicsFamily.value(), 0, &_GraphicsQueue);
        vkGetDeviceQueue(_Device, indices.PresentFamily.value(), 0, &_PresentQueue);
    }

    LogicalDevice::~LogicalDevice() noexcept
    {
        vkDestroyDevice(_Device, nullptr);
    }

    void LogicalDevice::PopulateDeviceQueueCreateInfo(VkDeviceQueueCreateInfo &createInfo, uint32_t index) noexcept
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueFamilyIndex = index;
        createInfo.queueCount = 1;

        float priority = 1.0f;
        createInfo.pQueuePriorities = &priority;
    }
}