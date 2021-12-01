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
    /// Queries the engine for graphics extensions required by GLFW
    /// and the engine itself.
    /// \return A non-empty list.
    _Magma_Dll std::vector<const char*> RequiredGraphicsExtensions();

    /// Wrapper around vkCreateDebugUtilsMessengerEXT.
    /// Gets proc address from the instance
    /// \return VK_ERROR_EXTENSION_NOT_PRESENT when VK_EXT_debug_utils is not present.
    _Magma_Dll VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    /// Wrapper around vkDestroyDebugUtilsMessengerEXT.
    /// Gets proc address from the instance
    /// \return VK_ERROR_EXTENSION_NOT_PRESENT when VK_EXT_debug_utils is not present.
    _Magma_Dll void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
}

#endif //MAGMA_EXT_H
