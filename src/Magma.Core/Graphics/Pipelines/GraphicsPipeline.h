//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_GRAPHICS_PIPELINE_H
#define MAGMA_GRAPHICS_PIPELINE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Graphics/Renderpass/Renderpass.h"
#include "Magma.Core/Graphics/Pipelines/Shader.h"
#include "Magma.Core/Graphics/Descriptors/DescriptorSet.h"

#include <filesystem>
#include <vulkan/vulkan.h>

namespace Magma
{
    class _Magma_Dll GraphicsPipeline
    {
    public:
        explicit GraphicsPipeline(const std::vector<std::filesystem::path>& shaderPaths, const Ref<Renderpass>& renderpass,
                                  VkVertexInputBindingDescription bindingDescription, std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions, const DescriptorSetLayout& layout);
        ~GraphicsPipeline();

        void AllocateDescriptorSets(DescriptorSet* pSets, const Ref<DescriptorSetLayout>& layout, uint32_t count = 0);

        [[nodiscard]] const VkPipeline& GetPipeline() const noexcept { return _Pipeline; }
        [[nodiscard]] const VkPipelineLayout& GetPipelineLayout() const noexcept { return _Layout; }
    private:
        VkPipelineLayout _Layout;
        VkPipeline _Pipeline;

        VkDescriptorPool _DescriptorPool;
    };
}

#endif //MAGMA_GRAPHICS_PIPELINE_H
