//
// Created by Acer on 7.11.2021 г..
//

#include "Magma.Core/Debug/ValidationLayers.h"
#include "Magma.Core/Devices/Ext.h"
#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Graphics/Graphics.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Magma
{
    const std::vector<const char*> RequiredValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    static VkDebugUtilsMessengerEXT DebugMessenger;

    std::vector<const char*> QueryValidationLayers()
    {
        return RequiredValidationLayers;
    }

    bool CheckValidationLayerSupport()
    {
        uint32_t iCount;
        vkEnumerateInstanceLayerProperties(&iCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(iCount);
        vkEnumerateInstanceLayerProperties(&iCount, availableLayers.data());

        for (const char* layerName : RequiredValidationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    VkBool32 VKAPI_CALL ValidationLayerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
    {
        switch(messageSeverity)
        {
            //case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: _Magma_Core_Trace(pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    _Magma_Core_Info(pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: _Magma_Core_Warn(pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:   _Magma_Core_Error(pCallbackData->pMessage); break;
        }

        return VK_FALSE;
    }

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = ValidationLayerCallback;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
    }

    void EnableValidationLayersOutput(VkInstance instance)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);

        Graphics::CheckVk(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &DebugMessenger));
    }

    void DestroyDebugUtilsMessenger(VkInstance instance)
    {
        DestroyDebugUtilsMessengerEXT(instance, DebugMessenger, nullptr);
    }
}