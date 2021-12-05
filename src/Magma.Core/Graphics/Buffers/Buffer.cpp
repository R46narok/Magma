//
// Created by Acer on 18.11.2021 г..
//

#include "Magma.Core/Graphics/Buffers/Buffer.h"
#include "Magma.Core/Graphics/Commands/CommandBuffer.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{

    Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props)
        : _deviceSize(size),
          _bufferMemory(VK_NULL_HANDLE),
          _buffer(VK_NULL_HANDLE)
    {
        VkBufferCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = size,
            .usage = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto physical = Graphics::GetPhysicalDevice()->GetVulkanPhysicalDevice();
        Graphics::CheckVk(vkCreateBuffer(device, &createInfo, nullptr, &_buffer));

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, _buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = FindMemoryType(physical, memRequirements.memoryTypeBits, props)
        };

        Graphics::CheckVk(vkAllocateMemory(device, &allocInfo, nullptr, &_bufferMemory));

        vkBindBufferMemory(device, _buffer, _bufferMemory, 0);
    }

    Buffer::~Buffer()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkDestroyBuffer(device, _buffer, nullptr);
        vkFreeMemory(device, _bufferMemory, nullptr);
    }

    uint32_t Buffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags props)
    {
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

        for(auto i = 0; i < memProps.memoryTypeCount; ++i)
        {
            if ((typeFilter & (1 << i)) &&
                (memProps.memoryTypes[i].propertyFlags & props)) return i;
        }

        return 0;
    }

    void* Buffer::MapMemory() const
    {
        void* pMemory;

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto result = vkMapMemory(device, _bufferMemory, 0, _deviceSize, 0, &pMemory);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not map buffer memory"));

        return pMemory;
    }

    void Buffer::UnmapMemory() const
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkUnmapMemory(device, _bufferMemory);
    }

    void Buffer::Copy(VkBuffer src, VkBuffer dst, VkDeviceSize size)
    {
        auto device = Graphics::GetDevice();
        CommandBuffer commandBuffer;

        commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkBufferCopy copyRegion{
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };

        commandBuffer.CopyBuffer(src, dst, 1, &copyRegion);
        commandBuffer.End();

        VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer.GetCommandBuffer()
        };

        // TODO: Implement fencing
        auto queue = device->GetGraphicsQueue();
        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }
}
