//
// Created by Acer on 12.11.2021 г..
//

#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Graphics/Renderpass/Renderpass.h"
#include "Magma.Core/Graphics/Renderpass/Framebuffer.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{
    Renderpass::Renderpass()
        : _Renderpass(VK_NULL_HANDLE)
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto swapchain = Graphics::GetSwapchain();

        VkAttachmentDescription colorAttachment{
            .format = swapchain->GetFormat(),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        VkAttachmentReference colorAttachmentRef{
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription subpass{
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef
        };

        VkSubpassDependency dependency{
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        };

        VkRenderPassCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = &colorAttachment,
            .subpassCount = 1,
            .pSubpasses = &subpass,
            .dependencyCount = 1,
            .pDependencies = &dependency
        };

        auto result = vkCreateRenderPass(device, &createInfo, nullptr, &_Renderpass);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not create renderpass"));
    }

    Renderpass::~Renderpass()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkDestroyRenderPass(device, _Renderpass, nullptr);
    }
}
