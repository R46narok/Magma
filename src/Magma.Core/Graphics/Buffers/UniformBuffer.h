//
// Created by Acer on 29.11.2021 г..
//

#ifndef MAGMA_UNIFORM_BUFFER_H
#define MAGMA_UNIFORM_BUFFER_H

#include "Magma.Core/Graphics/Buffers/Buffer.h"

namespace Magma
{
    class _Magma_Dll UniformBuffer : public Buffer
    {
    public:
        explicit UniformBuffer(uint32_t size);
    };
}

#endif //MAGMA_UNIFORM_BUFFER_H
