//
// Created by Acer on 29.11.2021 г..
//

#include "UniformBuffer.h"

namespace Magma
{
    UniformBuffer::UniformBuffer(uint32_t size)
        : Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {

    }

    VkDescriptorSetLayoutBinding
    UniformBuffer::GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count)
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
                .binding = binding,
                .descriptorType = descriptorType,
                .descriptorCount = 1,
                .stageFlags = stage,
                .pImmutableSamplers = nullptr
        };

        return descriptorSetLayoutBinding;
    }

}

