//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_RENDERPASS_H
#define MAGMA_RENDERPASS_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Graphics/Renderpass/Swapchain.h"
#include "Magma.Core/Devices/LogicalDevice.h"

#include <vector>
#include <vulkan/vulkan.h>
#include <array>

namespace Magma
{
    class Framebuffers;

    class MAGMA_API Renderpass
    {
    public:
        explicit Renderpass();
        ~Renderpass();

        [[nodiscard]] const VkRenderPass& GetVulkanRenderpass() const noexcept { return _Renderpass;}
    private:
        VkRenderPass _Renderpass;
    };
}

#endif //MAGMA_RENDERPASS_H
