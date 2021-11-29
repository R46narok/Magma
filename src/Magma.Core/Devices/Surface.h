//
// Created by Acer on 9.11.2021 г..
//

#ifndef MAGMA_SURFACE_H
#define MAGMA_SURFACE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Window.h"
#include "Magma.Core/Devices/Instance.h"
#include "QueueFamilies.h"

#include <vulkan/vulkan.h>

namespace Magma
{
    class _Magma_Dll Surface
    {
    public:
        explicit Surface();
        ~Surface() noexcept;

        [[nodiscard]] const VkSurfaceKHR& GetVulkanSurface() const noexcept { return _Surface; }
    private:
        VkSurfaceKHR _Surface;
    };
}

#endif //MAGMA_SURFACE_H
