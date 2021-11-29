//
// Created by Acer on 13.11.2021 г..
//

#include "Magma.Core/Graphics/Commands/CommandBuffer.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{
    //TODO: Create multiple buffers func
    CommandBuffer::CommandBuffer()
        : _Pool(Graphics::GetCommandPool())
    {
        VkCommandBufferAllocateInfo allocInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = _Pool->GetPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto result = vkAllocateCommandBuffers(device, &allocInfo, &_Buffer);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not allocate command buffer"));
    }

    CommandBuffer::~CommandBuffer()
    = default;

    void CommandBuffer::Begin(VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo beginInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
          .flags = flags,
          .pInheritanceInfo = nullptr
        };

        auto result = vkBeginCommandBuffer(_Buffer, &beginInfo);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not begin command buffer"));
    }

    void CommandBuffer::End()
    {
        auto result = vkEndCommandBuffer(_Buffer);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not end command buffer"));
    }

    void CommandBuffer::CopyBuffer(VkBuffer src, VkBuffer dst, uint32_t regionCount, VkBufferCopy *pCopyRegions)
    {
        vkCmdCopyBuffer(_Buffer, src, dst, regionCount, pCopyRegions);
    }

    void CommandBuffer::BindPipeline(const Ref<GraphicsPipeline>& pipeline)
    {
        vkCmdBindPipeline(_Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
    }

    void CommandBuffer::BeginRenderpass(const Ref<Renderpass> &renderpass, VkFramebuffer framebuffer)
    {
        auto extent = Graphics::GetSwapchain()->GetExtent2D();

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        VkRenderPassBeginInfo info{
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = renderpass->GetVulkanRenderpass(),
                .framebuffer = framebuffer,
                .renderArea = {.offset = {0, 0}, .extent = extent},
                .clearValueCount = 1,
                .pClearValues = &clearColor
        };

        vkCmdBeginRenderPass(_Buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderpass()
    {
        vkCmdEndRenderPass(_Buffer);
    }

    void CommandBuffer::BindVertexBuffer(const Ref<Buffer> &buffer, VkDeviceSize *pOffset)
    {
        vkCmdBindVertexBuffers(_Buffer, 0, 1, &buffer->GetBuffer(), pOffset);
    }

    void CommandBuffer::BindIndexBuffer(const Ref<Buffer> &buffer, VkIndexType indexType, uint32_t offset)
    {
        vkCmdBindIndexBuffer(_Buffer, buffer->GetBuffer(), offset, indexType);
    }

    void CommandBuffer::DrawIndexed(uint32_t indicesCount)
    {
        vkCmdDrawIndexed(_Buffer, indicesCount, 1, 0, 0, 0);
    }
}
