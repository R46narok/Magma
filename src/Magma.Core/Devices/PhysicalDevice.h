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

    class MAGMA_API PhysicalDevice
    {
    public:
        /// Chooses the best graphics card with Vulkan support in the system.
        /// TODO: Pick slower gpu if the device is on battery.
        /// \param instance A valid Vulkan Instance.
        explicit PhysicalDevice(const Ref<Instance>& instance);

        [[nodiscard]] const VkPhysicalDevice& GetVulkanPhysicalDevice() const noexcept { return _PhysicalDevice; }
    private:
        static VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);

        /// Scores physical devices based on their functionality support,
        /// device type(discrete GPUs are given the highest score) and max 2D image dimension.
        /// \param device A valid graphics card installed in the system.
        /// \return 0 if some functionality required by the engine is not supported.
        static uint32_t ScorePhysicalDevice(const VkPhysicalDevice& device);

        /// Checks for device specific extensions required by the engine.
        /// \param device A valid graphics card installed in the system.
        /// \return true if all extensions are supported.
        static bool CheckExtensionSupport(const VkPhysicalDevice& device);
    private:
        VkPhysicalDevice _PhysicalDevice;
        VkPhysicalDeviceProperties _Properties{};
        VkPhysicalDeviceFeatures _Features{};
        VkPhysicalDeviceMemoryProperties _MemoryProperties{};
    };
}

#endif //MAGMA_PHYSICAL_DEVICE_H
