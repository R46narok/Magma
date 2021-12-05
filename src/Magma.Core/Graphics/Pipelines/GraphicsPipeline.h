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
    class MAGMA_API GraphicsPipeline
    {
    public:
        explicit GraphicsPipeline(const std::vector<std::filesystem::path>& shaders,
                                  const Ref<Renderpass>& renderpass,
                                  const std::vector<Shader::VertexInput>& vertexInputs);
        ~GraphicsPipeline();

        void AllocateDescriptorSets(DescriptorSet* pSets, const Ref<DescriptorSetLayout>& layout, uint32_t count = 0);

        [[nodiscard]] const VkPipeline& GetPipeline() const noexcept { return _Pipeline; }
        [[nodiscard]] const VkPipelineLayout& GetPipelineLayout() const noexcept { return _Layout; }
    private:
        void CreateShaderProgram();
        void CreateDescriptorLayout();
        void CreateDescriptorPool();
        void CreatePipelineLayout();
        void CreatePipeline();
    private:
        Scope<Shader> m_Shader;

        std::vector<std::filesystem::path> m_ShaderStages;
        std::vector<Shader::VertexInput> m_VertexInputs;
        std::vector<VkShaderModule> m_Modules;
        std::vector<VkPipelineShaderStageCreateInfo> m_Stages;

        VkPipelineLayout _Layout;
        VkPipeline _Pipeline;

        VkDescriptorPool _DescriptorPool;
    };
}

#endif //MAGMA_GRAPHICS_PIPELINE_H
