//
// Created by Acer on 8.11.2021 г..
//

#include "Magma.Core/Devices/PhysicalDevice.h"
#include "Magma.Core/Graphics/Renderpass/Swapchain.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"

#include <vector>
#include <map>
#include <set>
#include <string>

namespace Magma
{
    PhysicalDevice::PhysicalDevice(const Ref<Instance>& instance)
        : _PhysicalDevice(VK_NULL_HANDLE)
    {
        uint32_t iDeviceCount = 0;
        vkEnumeratePhysicalDevices(instance->GetVulkanInstance(), &iDeviceCount, nullptr);
        _Magma_Assert(iDeviceCount != 0, _Magma_Core_Error("No Vulkan compatible device found"));

        std::vector<VkPhysicalDevice> devices(iDeviceCount);
        vkEnumeratePhysicalDevices(instance->GetVulkanInstance(), &iDeviceCount, devices.data());
        _PhysicalDevice = ChoosePhysicalDevice(devices);

        vkGetPhysicalDeviceProperties(_PhysicalDevice, &_Properties);
        vkGetPhysicalDeviceFeatures(_PhysicalDevice, &_Features);
        vkGetPhysicalDeviceMemoryProperties(_PhysicalDevice, &_MemoryProperties);

        _Magma_Core_Info("Using GPU: {0}", _Properties.deviceName);
        _Magma_Core_Info("Properties: \n"
                         "                 limits.maxMemoryAllocationCount: {0}\n"
                         "                 limits.maxPushConstantsSize: {1}",
                         _Properties.limits.maxMemoryAllocationCount,
                         _Properties.limits.maxPushConstantsSize);
    }


    VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices)
    {
        std::multimap<uint32_t, VkPhysicalDevice> rankedDevices;

        for (auto& device : devices)
            rankedDevices.emplace(ScorePhysicalDevice(device), device);

        if (rankedDevices.rbegin()->first > 0)
            return rankedDevices.rbegin()->second;

        return nullptr;
    }

    uint32_t PhysicalDevice::ScorePhysicalDevice(const VkPhysicalDevice& device)
    {
        VkPhysicalDeviceProperties props;
        VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceProperties(device, &props);
        vkGetPhysicalDeviceFeatures(device, &features);

        auto surface = Graphics::GetSurface()->GetVulkanSurface();
        auto families = FindQueueFamilies(device, surface);
        auto extensionSupport = CheckExtensionSupport(device);
        auto swapchainSupport = false;

        if (extensionSupport)
        {
            auto swapchainDetails = QuerySwapchainSupport(device, surface);
            swapchainSupport = !(swapchainDetails.Formats.empty() && swapchainDetails.PresentModes.empty());
        }

        if (!(families.IsComplete() && extensionSupport && swapchainSupport))
            return 0;

        uint32_t iScore = 0;
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            iScore += 1000;

        iScore += props.limits.maxImageDimension2D;

        return iScore;
    }

    bool PhysicalDevice::CheckExtensionSupport(const VkPhysicalDevice& device)
    {
        uint32_t iCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &iCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(iCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &iCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(RequiredDeviceExtensions.begin(), RequiredDeviceExtensions.end());

        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);

        return requiredExtensions.empty();
    }

}