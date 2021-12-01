//
// Created by Acer on 7.11.2021 г..
//

#ifndef SMARTCITY_VALIDATION_LAYERS_H
#define SMARTCITY_VALIDATION_LAYERS_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/Instance.h"

#include <vector>

struct VkDebugUtilsMessengerCreateInfoEXT;

namespace Magma
{
    /// Enables validation layers to write to the standard output and file.
    /// Displays output from message with severity greater than info.
    /// \param instance A valid VkInstance.
    _Magma_Dll void EnableValidationLayersOutput(VkInstance instance);

    /// Enumerates the instance for available layers and checks
    /// if all required by the engine validation layers are enabled.
    /// \return true if all layers are present.
    _Magma_Dll bool CheckValidationLayerSupport();

    /// Queries the engine for required extensions.
    /// \return A non-empty list.
    _Magma_Dll std::vector<const char*> QueryValidationLayers();

    /// Populates a VkDebugUtilsMessengerCreateInfoEXT structure to
    /// not filter any message types and ignore messages with severity less than verbose.
    /// \param createInfo Structure to be populated.
    _Magma_Dll void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    _Magma_Dll void DestroyDebugUtilsMessenger(VkInstance instance);
}

#endif //MAGMA_VALIDATION_LAYERS_H
