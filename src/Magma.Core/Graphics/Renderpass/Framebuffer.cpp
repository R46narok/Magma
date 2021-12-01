//
// Created by Acer on 12.11.2021 г..
//

#include "Magma.Core/Graphics/Renderpass/Framebuffer.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{
    Framebuffers::Framebuffers(const Ref<Renderpass>& renderpass)
        : _Framebuffers()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto swapchain = Graphics::GetSwapchain();

        auto extent = swapchain->GetExtent2D();
        const auto& imageViews = swapchain->GetImageViews();
        _Framebuffers.resize(imageViews.size());

        for(int i = 0; i < imageViews.size(); ++i)
        {
            VkImageView attachments[]{
                imageViews[i]
            };

            VkFramebufferCreateInfo createInfo{
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = renderpass->GetVulkanRenderpass(),
                .attachmentCount = 1,
                .pAttachments = attachments,
                .width = extent.width,
                .height = extent.height,
                .layers = 1
            };

            auto result = vkCreateFramebuffer(device, &createInfo, nullptr, &_Framebuffers[i]);
            _Magma_VkAssert(result, _Magma_Core_Error("Could not create framebuffer"));
        }
    }

    Framebuffers::~Framebuffers() noexcept
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        for (auto framebuffer : _Framebuffers)
            vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

