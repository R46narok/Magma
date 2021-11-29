//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_GRAPHICS_PIPELINE_H
#define MAGMA_GRAPHICS_PIPELINE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Graphics/Pipelines/Shader.h"

#include <filesystem>
#include <vulkan/vulkan.h>

namespace Magma
{
    class _Magma_Dll GraphicsPipeline
    {
    public:
        explicit GraphicsPipeline(const std::vector<std::filesystem::path>& shaderPaths, VkDescriptorSetLayout descriptorSetLayout);
        ~GraphicsPipeline();

        const VkPipeline& GetPipeline() const noexcept { return _Pipeline; }
        const VkPipelineLayout& GetPipelineLayout() const noexcept { return _Layout; }
    private:
        VkPipelineLayout _Layout;
        VkPipeline _Pipeline;
    };
}

#endif //MAGMA_GRAPHICS_PIPELINE_H
