//
// Created by Acer on 13.11.2021 г..
//

#ifndef MAGMA_COMMAND_BUFFER_H
#define MAGMA_COMMAND_BUFFER_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Graphics/Commands/CommandPool.h"
#include "Magma.Core/Graphics/Pipelines/GraphicsPipeline.h"
#include "Magma.Core/Graphics/Renderpass/Renderpass.h"
#include "Magma.Core/Graphics/Buffers/Buffer.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    class _Magma_Dll CommandBuffer
    {
    public:
        explicit CommandBuffer();
        ~CommandBuffer();

        [[nodiscard]] const VkCommandBuffer& GetCommandBuffer() const noexcept { return _Buffer; }

        void Begin(VkCommandBufferUsageFlags flags = 0);
        void End();

        #pragma region Buffer Commands

        void BindVertexBuffer(const Ref<Buffer>& buffer, VkDeviceSize* pOffset);
        void BindIndexBuffer(const Ref<Buffer>& buffer, VkIndexType indexType, uint32_t offset = 0);
        void CopyBuffer(VkBuffer src, VkBuffer dst, uint32_t regionCount, VkBufferCopy *pCopyRegions);

        #pragma endregion

        #pragma region Pipeline Commands

        void BindPipeline(const Ref<GraphicsPipeline>& pipeline);

        #pragma endregion

        #pragma region Renderpass Commands

        void BeginRenderpass(const Ref<Renderpass>& renderpass, VkFramebuffer framebuffer);
        void EndRenderpass();

        #pragma endregion

        #pragma region Drawing Commands

        void DrawIndexed(uint32_t indicesCount);

        #pragma endregion
    private:
        VkCommandBuffer _Buffer;
        const CommandPool* _Pool;
    };
}

#endif //MAGMA_COMMAND_BUFFER_H
