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
        GraphicsPipeline(const std::vector<std::filesystem::path>& shaders,
                                  const Ref<Renderpass>& renderpass,
                                  const std::vector<Shader::VertexInput>& vertexInputs);
        ~GraphicsPipeline();

        void AllocateDescriptorSets(DescriptorSet* pSets, const Ref<DescriptorSetLayout>& layout, uint32_t count = 0);

        [[nodiscard]] const VkPipeline& GetPipeline() const noexcept { return m_Pipeline; }
        [[nodiscard]] const VkPipelineLayout& GetPipelineLayout() const noexcept { return m_PipelineLayout; }
    private:
        void CreateShaderProgram();
        void CreateDescriptorLayout();
        void CreateDescriptorPool();
        void CreatePipelineLayout();
        void CreateAttributes();
        void CreatePipeline();
    private:
        VkRenderPass m_Renderpass;
        Scope<Shader> m_Shader;

        std::vector<std::filesystem::path> m_ShaderStages;
        std::vector<Shader::VertexInput> m_VertexInputs;
        std::vector<VkShaderModule> m_Modules;
        std::vector<VkPipelineShaderStageCreateInfo> m_Stages;

        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_Pipeline;

        VkDescriptorSetLayout m_DescriptorSetLayout;
        VkDescriptorPool m_DescriptorPool;

        VkPipelineVertexInputStateCreateInfo m_VertexInputState = {};
        VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyState = {};
        VkPipelineRasterizationStateCreateInfo m_RasterizationState = {};
        VkPipelineColorBlendAttachmentState m_ColorBlendAttachmentState = {};
        VkPipelineColorBlendStateCreateInfo m_ColorBlendState = {};
        VkPipelineDepthStencilStateCreateInfo m_DepthStencilState = {};
        VkPipelineViewportStateCreateInfo m_ViewportState = {};
        VkPipelineMultisampleStateCreateInfo m_MultisampleState = {};

        VkViewport m_Viewport;
        VkRect2D m_Scissor;
    };
}

#endif //MAGMA_GRAPHICS_PIPELINE_H
