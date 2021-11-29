//
// Created by Asus on 8.11.2021 г..
//

#include "Magma.Core/Devices/Ext.h"

#include <array>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Magma
{
    const std::array<const char*, 1> OptionalExtensions = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    std::vector<const char*> RequiredGraphicsExtensions()
    {
        uint32_t iGlfwExtensionCount = 0;
        const char** ppGlfwExtensions = glfwGetRequiredInstanceExtensions(&iGlfwExtensionCount);
        std::vector<const char*> extensions(ppGlfwExtensions, ppGlfwExtensions + iGlfwExtensionCount);

        for(auto& e : OptionalExtensions)
            extensions.push_back(e);

        return extensions;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }
}