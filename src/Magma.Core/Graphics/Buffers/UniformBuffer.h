//
// Created by Acer on 29.11.2021 г..
//

#ifndef MAGMA_UNIFORM_BUFFER_H
#define MAGMA_UNIFORM_BUFFER_H

#include "Magma.Core/Graphics/Buffers/Buffer.h"

namespace Magma
{
    class MAGMA_API UniformBuffer : public Buffer
    {
    public:
        explicit UniformBuffer(uint32_t size);

        static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);
    };
}

#endif //MAGMA_UNIFORM_BUFFER_H
