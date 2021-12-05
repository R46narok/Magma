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
    /// Base class for all buffers on the VRAM.
    class MAGMA_API Buffer
    {
    public:
        /// Creates a Vulkan buffer and allocates memory for it.
        /// \param size A valid size, greater than 0.
        /// \param usage The buffer type.
        /// \param props Properties for the allocated memory.
        explicit Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);
        ~Buffer();

        explicit operator const VkBuffer&() const { return _buffer; }

        /// TODO: Implicitly uses a staging buffer, if the above memory properties
        /// are set to VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT(not mappable from the CPU).
        /// \return A mapped memory address on the CPU side.
        [[nodiscard]] void* MapMemory() const;

        /// Flushes the data to the buffer.
        void UnmapMemory() const;

        [[nodiscard]] const VkDeviceSize& GetSize() const noexcept { return _deviceSize; }
        [[nodiscard]] const VkBuffer& GetBuffer() const noexcept { return _buffer;}
        [[nodiscard]] const VkDeviceMemory& GetBufferMemory() const noexcept { return _bufferMemory; }

        /// Finds the memory type index of a GPU.
        /// \param physicalDevice A valid GPU.
        /// \param typeFilter Bitmask for every supported type.
        /// \param props Memory type to find the index of.
        /// \return 0 if the memory type index is not found.
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
