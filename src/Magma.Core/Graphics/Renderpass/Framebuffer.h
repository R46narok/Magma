//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_FRAMEBUFFER_H
#define MAGMA_FRAMEBUFFER_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <vector>
#include <vulkan/vulkan.h>
#include <array>

namespace Magma
{
    class _Magma_Dll Framebuffers
    {
    public:
        explicit Framebuffers();
        ~Framebuffers();

        [[nodiscard]] const std::vector<VkFramebuffer>& GetFramebuffers() const noexcept { return _Framebuffers; }
    private:
        std::vector<VkFramebuffer> _Framebuffers;
    };
}

#endif //MAGMA_FRAMEBUFFER_H
