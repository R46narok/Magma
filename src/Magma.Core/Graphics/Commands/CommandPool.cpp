//
// Created by Acer on 13.11.2021 г..
//

#include "Magma.Core/Graphics/Commands/CommandPool.h"
#include "Magma.Core/Graphics/Graphics.h"

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

        Graphics::CheckVk(vkCreateCommandPool(device->GetVulkanDevice(), &createInfo, nullptr, &_CommandPool));
    }

    CommandPool::~CommandPool()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkDestroyCommandPool(device, _CommandPool, nullptr);
    }
}

