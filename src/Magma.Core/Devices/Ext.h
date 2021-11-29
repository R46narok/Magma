//
// Created by Asus on 8.11.2021 г..
//

#ifndef MAGMA_EXT_H
#define MAGMA_EXT_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <vector>
#include <vulkan/vulkan.h>

namespace Magma
{
    _Magma_Dll std::vector<const char*> RequiredGraphicsExtensions();

    _Magma_Dll VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    _Magma_Dll void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
}

#endif //MAGMA_EXT_H
