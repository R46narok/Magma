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
        explicit LogicalDevice();
        ~LogicalDevice() noexcept;

        explicit operator const VkDevice &() const { return _Device; }
        explicit operator VkDevice &() { return _Device; }

        [[nodiscard]] const VkDevice& GetVulkanDevice() const noexcept { return _Device; }

        [[nodiscard]] const VkQueue& GetGraphicsQueue() const noexcept { return _GraphicsQueue; }
    private:
        static void PopulateDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& createInfo, uint32_t index) noexcept;
    private:
        VkDevice _Device;
        VkQueue _GraphicsQueue;
        VkQueue _PresentQueue;
    };
}

#endif //MAGMA_LOGICAL_DEVICE_H
