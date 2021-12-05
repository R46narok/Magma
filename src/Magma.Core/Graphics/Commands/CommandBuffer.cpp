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
        : _Pool(Graphics::GetCommandPool()),
          _Buffer(VK_NULL_HANDLE)
    {
        VkCommandBufferAllocateInfo allocInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = _Pool->GetPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        Graphics::CheckVk(vkAllocateCommandBuffers(device, &allocInfo, &_Buffer));
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

        Graphics::CheckVk(vkBeginCommandBuffer(_Buffer, &beginInfo));
    }

    void CommandBuffer::End()
    {
        Graphics::CheckVk(vkEndCommandBuffer(_Buffer));
    }

    void CommandBuffer::CopyBuffer(VkBuffer src, VkBuffer dst, uint32_t regionCount, VkBufferCopy *pCopyRegions)
    {
        vkCmdCopyBuffer(_Buffer, src, dst, regionCount, pCopyRegions);
    }

    void CommandBuffer::BindPipeline(const Ref<GraphicsPipeline>& pipeline)
    {
        vkCmdBindPipeline(_Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
    }

    void CommandBuffer::BeginRenderpass(const Ref<Renderpass> &renderpass, VkFramebuffer framebuffer, VkClearValue clear)
    {
        auto extent = Graphics::GetSwapchain()->GetExtent2D();

        VkRenderPassBeginInfo info{
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = renderpass->GetVulkanRenderpass(),
                .framebuffer = framebuffer,
                .renderArea = {.offset = {0, 0}, .extent = extent},
                .clearValueCount = 1,
                .pClearValues = &clear
        };

        vkCmdBeginRenderPass(_Buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderpass()
    {
        vkCmdEndRenderPass(_Buffer);
    }

    void CommandBuffer::BindVertexBuffer(const Ref<Buffer> &buffer, VkDeviceSize offset)
    {
        vkCmdBindVertexBuffers(_Buffer, 0, 1, &buffer->GetBuffer(), &offset);
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
