//
// Created by Acer on 7.11.2021 г..
//

#ifndef SMARTCITY_INSTANCE_H
#define SMARTCITY_INSTANCE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

_Magma_Define_VkHandle(VkInstance);

namespace Magma
{
    class _Magma_Dll Instance
    {
    public:
        Instance();
        ~Instance() noexcept;

        explicit operator const VkInstance &() const { return _Instance; }

        [[nodiscard]] constexpr VkInstance GetVulkanInstance() const noexcept { return _Instance; }
    private:
        VkInstance _Instance;
    };
}

#endif //SMARTCITY_INSTANCE_H