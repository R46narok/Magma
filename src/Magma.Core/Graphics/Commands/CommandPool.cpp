//
// Created by Acer on 13.11.2021 г..
//

#include "Magma.Core/Graphics/Commands/CommandPool.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Devices/QueueFamilies.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{
    CommandPool::CommandPool(const Ref<PhysicalDevice>& physicalDevice, const Ref<LogicalDevice>& device, const Ref<Surface>& surface)
        : _CommandPool(VK_NULL_HANDLE)
    {
        auto indices = FindQueueFamilies(physicalDevice->GetVulkanPhysicalDevice(), surface->GetVulkanSurface());

        VkCommandPoolCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .queueFamilyIndex = indices.GraphicsFamily.value()
        };

        auto result = vkCreateCommandPool(device->GetVulkanDevice(), &createInfo, nullptr, &_CommandPool);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not create command pool"));
    }

    CommandPool::~CommandPool()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkDestroyCommandPool(device, _CommandPool, nullptr);
    }
}

