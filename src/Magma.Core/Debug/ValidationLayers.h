//
// Created by Acer on 7.11.2021 г..
//

#ifndef SMARTCITY_VALIDATIONLAYERS_H
#define SMARTCITY_VALIDATIONLAYERS_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/Instance.h"

#include <vector>

struct VkDebugUtilsMessengerCreateInfoEXT;

namespace Magma
{
    _Magma_Dll void EnableValidationLayersOutput(VkInstance instance);
    _Magma_Dll bool CheckValidationLayerSupport();
    _Magma_Dll std::vector<const char*> QueryValidationLayers();

    _Magma_Dll void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    _Magma_Dll void DestroyDebugUtilsMessenger(VkInstance instance);
}

#endif //SMARTCITY_VALIDATIONLAYERS_H
