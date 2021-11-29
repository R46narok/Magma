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
    class _Magma_Dll CommandPool
    {
    public:
        explicit CommandPool(const Ref<PhysicalDevice>& physicalDevice, const Ref<LogicalDevice>& device, const Ref<Surface>& surface);
        ~CommandPool();

        [[nodiscard]] const VkCommandPool& GetPool() const noexcept { return _CommandPool; }
    private:
        VkCommandPool _CommandPool;
    };
}

#endif //MAGMA_COMMANDPOOL_H
