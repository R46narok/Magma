//
// Created by Acer on 9.11.2021 г..
//

#ifndef MAGMA_QUEUE_FAMILIES_H
#define MAGMA_QUEUE_FAMILIES_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <optional>
#include <vulkan/vulkan.h>

_Magma_Define_VkHandle(VkPhysicalDevice);

namespace Magma
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;
        std::optional<uint32_t> ComputeFamily;

        [[nodiscard]] constexpr bool IsComplete() const noexcept
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value() && ComputeFamily.has_value();
        }
    };

    _Magma_Dll QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}

#endif //MAGMA_QUEUE_FAMILIES_H
