//
// Created by Acer on 16.11.2021 г..
//

#ifndef MAGMA_RENDERER2D_H
#define MAGMA_RENDERER2D_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Engine/Module.h"

namespace Magma
{
    class Graphics;

    class _Magma_Dll Renderer2D
    {
    public:
        Renderer2D();
        ~Renderer2D();

        void Update();
    };
}

#endif //MAGMA_RENDERER2D_H
