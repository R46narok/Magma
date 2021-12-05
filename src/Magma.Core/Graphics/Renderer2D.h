//
// Created by Acer on 16.11.2021 г..
//

#ifndef MAGMA_RENDERER2D_H
#define MAGMA_RENDERER2D_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Engine/Module.h"
#include "Magma.Core/Graphics/Renderpass/Renderpass.h"
#include "Magma.Core/Graphics/Pipelines/GraphicsPipeline.h"
#include "Magma.Core/Graphics/Commands/CommandBuffer.h"
#include "Magma.Core/Graphics/Buffers/Buffer.h"
#include "Magma.Core/Graphics/Buffers/UniformBuffer.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    class Graphics;

    class MAGMA_API Renderer2D
    {
    public:
        Renderer2D();
        ~Renderer2D();

        void Update();
        void UpdateUniformBuffer(uint32_t currentImage);
    private:
        Ref<Renderpass> _Renderpass;
        Ref<GraphicsPipeline> _Pipeline;
        Ref<Framebuffers> _Framebuffers;

        std::vector<VkSemaphore> _ImageAvailableSemaphore;
        std::vector<VkSemaphore> _RenderFinishedSemaphore;
        std::vector<VkFence> _InFlightFences;
        std::vector<VkFence> _ImagesInFlight;
        std::vector<CommandBuffer> _CommandBuffers;


        Ref<Buffer> _VertexBuffer;
        Ref<Buffer> _IndexBuffer;
    };
}

#endif //MAGMA_RENDERER2D_H
