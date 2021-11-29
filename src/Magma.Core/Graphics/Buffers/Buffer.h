//
// Created by Acer on 18.11.2021 г..
//

#ifndef MAGMA_BUFFER_H
#define MAGMA_BUFFER_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/LogicalDevice.h"
#include "Magma.Core/Graphics/Commands/CommandPool.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    class _Magma_Dll Buffer
    {
    public:
        explicit Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);
        ~Buffer();

        explicit operator const VkBuffer&() const { return _buffer; }

        [[nodiscard]] void* MapMemory() const;
        void UnmapMemory() const;

        [[nodiscard]] const VkDeviceSize& GetSize() const noexcept { return _deviceSize; }
        [[nodiscard]] const VkBuffer& GetBuffer() const noexcept { return _buffer;}
        [[nodiscard]] const VkDeviceMemory& GetBufferMemory() const noexcept { return _bufferMemory; }

        static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags props);

        // TODO: Use separate command pool with VK_COMMAND_POOL_TRANSIENT_BIT
        static void Copy(VkBuffer src, VkBuffer dst, VkDeviceSize size);

    protected:
        VkBuffer _buffer;
        VkDeviceMemory _bufferMemory;
        VkDeviceSize _deviceSize;
    };
}

#endif //MAGMA_BUFFER_H
