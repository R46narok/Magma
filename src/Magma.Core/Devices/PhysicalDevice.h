//
// Created by Acer on 8.11.2021 г..
//

#ifndef MAGMA_PHYSICAL_DEVICE_H
#define MAGMA_PHYSICAL_DEVICE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/Instance.h"
#include "Magma.Core/Devices/Surface.h"

#include <vector>
#include <vulkan/vulkan.h>
#include <array>

namespace Magma
{
    constexpr std::array<const char*, 1> RequiredDeviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    class _Magma_Dll PhysicalDevice
    {
    public:
        explicit PhysicalDevice();

        [[nodiscard]] const VkPhysicalDevice& GetVulkanPhysicalDevice() const noexcept { return _PhysicalDevice; }
    private:
        static VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);
        static uint32_t ScorePhysicalDevice(const VkPhysicalDevice& device);
        static bool CheckExtensionSupport(const VkPhysicalDevice& device);
    private:
        VkPhysicalDevice _PhysicalDevice;
        VkPhysicalDeviceProperties _Properties{};
        VkPhysicalDeviceFeatures _Features{};
        VkPhysicalDeviceMemoryProperties _MemoryProperties{};
    };
}

#endif //MAGMA_PHYSICAL_DEVICE_H
