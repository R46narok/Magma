//
// Created by Acer on 9.11.2021 г..
//

#ifndef MAGMA_LOGICAL_DEVICE_H
#define MAGMA_LOGICAL_DEVICE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/Instance.h"
#include "Magma.Core/Devices/PhysicalDevice.h"
#include "Magma.Core/Devices/Surface.h"
#include "Magma.Core/Devices/QueueFamilies.h"

namespace Magma
{
    class _Magma_Dll LogicalDevice
    {
        friend class Graphics;
    public:
        /// Creates the basic mechanism for Vulkan communication(VkDevice) and
        /// all queue types required by the engine.
        /// \param physicalDevice GPU to wrap.
        /// \param surface Used to get queue family indices.
        LogicalDevice(const Ref<PhysicalDevice>& physicalDevice, const Ref<Surface>& surface);
        ~LogicalDevice() noexcept;

        explicit operator const VkDevice &() const { return _Device; }
        explicit operator VkDevice &() { return _Device; }

        [[nodiscard]] const VkDevice& GetVulkanDevice() const noexcept { return _Device; }

        [[nodiscard]] const VkQueue& GetGraphicsQueue() const noexcept { return _GraphicsQueue; }
        [[nodiscard]] const VkQueue& GetPresentQueue() const noexcept { return _PresentQueue; }
    private:
        /// Populates VkDeviceQueueCreateInfo and does not the particular queue any priority.
        /// One queue is set for creation.
        /// \param createInfo Structure to populate.
        /// \param index Queue family index.
        static void PopulateDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& createInfo, uint32_t index) noexcept;
    private:
        VkDevice _Device;
        VkQueue _GraphicsQueue;
        VkQueue _PresentQueue;
    };
}

#endif //MAGMA_LOGICAL_DEVICE_H
