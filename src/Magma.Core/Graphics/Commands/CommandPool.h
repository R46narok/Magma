//
// Created by Acer on 13.11.2021 г..
//

#ifndef MAGMA_COMMANDPOOL_H
#define MAGMA_COMMANDPOOL_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/PhysicalDevice.h"
#include "Magma.Core/Devices/LogicalDevice.h"
#include "Magma.Core/Devices/Surface.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    class MAGMA_API CommandPool
    {
    public:
        /// Creates a Vulkan command pool on the graphics queue family TODO
        /// \param physicalDevice A valid GPU to get the queue family index.
        /// \param device A valid Vulkan device to create the pool on.
        /// \param surface A valid surface to get the queue family index. TODO: Cache this stuff, no need to pass the physical device and surface here
        explicit CommandPool(const Ref<PhysicalDevice>& physicalDevice, const Ref<LogicalDevice>& device, const Ref<Surface>& surface);
        ~CommandPool();

        [[nodiscard]] const VkCommandPool& GetPool() const noexcept { return _CommandPool; }
    private:
        VkCommandPool _CommandPool;
    };
}

#endif //MAGMA_COMMANDPOOL_H
