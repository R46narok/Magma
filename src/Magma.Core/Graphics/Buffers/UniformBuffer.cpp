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

}

