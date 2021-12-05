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
    /// Buffer, which contains information about Vulkan commands and their
    /// sequence.
    class MAGMA_API CommandBuffer
    {
    public:
        /// Creates a primary level command buffer from the command pool
        /// of the current thread.
        explicit CommandBuffer();
        ~CommandBuffer();

        [[nodiscard]] const VkCommandBuffer& GetCommandBuffer() const noexcept { return _Buffer; }

        /// Prepares the command buffer for submitting.
        /// \param flags 0 is none, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT is a common value.
        void Begin(VkCommandBufferUsageFlags flags = 0);

        /// Stops submitting.
        void End();

        #pragma region Buffer Commands

        /// Binds only one vertex buffer.
        /// TODO: Extend it
        /// \param buffer A valid vertex buffer on the GPU.
        /// \param offset Leave empty to use the entire buffer from its beginning.
        void BindVertexBuffer(const Ref<Buffer>& buffer, VkDeviceSize offset = 0);

        /// Binds only one index buffer.
        /// \param buffer A valid index buffer on the GPU.
        /// \param indexType
        /// \param offset Leave empty to use the entire buffer from its beginning.
        void BindIndexBuffer(const Ref<Buffer>& buffer, VkIndexType indexType, uint32_t offset = 0);

        /// Copies regionCount amount of regions from one buffer to another.
        /// \param src A valid buffer to copy from.
        /// \param dst A valid buffer to copy to.
        /// \param regionCount Number of regions.
        /// \param pCopyRegions Valid pointer to regionCount buffer regions.
        void CopyBuffer(VkBuffer src, VkBuffer dst, uint32_t regionCount, VkBufferCopy *pCopyRegions);

        #pragma endregion

        #pragma region Pipeline Commands

        void BindPipeline(const Ref<GraphicsPipeline>& pipeline);

        #pragma endregion

        #pragma region Renderpass Commands

        /// Begins a renderpass on a particular framebuffer using the entire swapchain extent.
        /// \param renderpass A valid renderpass to begin.
        /// \param framebuffer Framebuffer to use.
        /// \param clear Color and depth stencil to clear.
        void BeginRenderpass(const Ref<Renderpass>& renderpass, VkFramebuffer framebuffer, VkClearValue clear);
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
