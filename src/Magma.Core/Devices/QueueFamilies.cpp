//
// Created by Acer on 9.11.2021 г..
//

#include "Magma.Core/Devices/QueueFamilies.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Magma
{
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        uint32_t iCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &iCount, nullptr);

        std::vector<VkQueueFamilyProperties> families(iCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &iCount, families.data());

        for (int i = 0; i < families.size(); ++i)
        {
            if (indices.IsComplete()) break;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
                indices.PresentFamily = i;

            if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.GraphicsFamily = i;
            if (families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                indices.ComputeFamily = i;
        }

        return indices;
    }
}