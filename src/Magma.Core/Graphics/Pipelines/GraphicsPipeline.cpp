//
// Created by Acer on 12.11.2021 г..
//

#include "Magma.Core/Graphics/Pipelines/GraphicsPipeline.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Filesystem/File.h"



#include <array>
#include <fstream>

namespace Magma
{

    std::optional<std::string> ReadF(const std::filesystem::path &path)
    {
        std::ifstream file(path);
        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));

        return content;
    }

    GraphicsPipeline::GraphicsPipeline(const std::vector<std::filesystem::path>& shaders,
                                       const Ref<Renderpass>& renderpass,
                                       const std::vector<Shader::VertexInput>& vertexInputs)
            :
            m_Renderpass(renderpass->GetVulkanRenderpass()),
            m_Shader(CreateScope<Shader>()),
            m_ShaderStages(shaders),
            m_VertexInputs(vertexInputs),
            m_PipelineLayout(VK_NULL_HANDLE),
            m_Pipeline(VK_NULL_HANDLE)
    {
        std::sort(m_VertexInputs.begin(), m_VertexInputs.end());

        CreateShaderProgram();
        CreateDescriptorLayout();
        CreateDescriptorPool();
        CreatePipelineLayout();
        CreateAttributes();
        CreatePipeline();
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        //vkDestroyPipeline(device, _Pipeline, nullptr);
        //vkDestroyPipelineLayout(device, _Layout, nullptr);
    }

    void GraphicsPipeline::AllocateDescriptorSets(DescriptorSet* pSets, const Ref<DescriptorSetLayout>& layout, uint32_t count)
    {
        if (count == 0)
            count = Graphics::GetSwapchain()->GetImageCount();

        std::vector<VkDescriptorSetLayout> layouts(count, layout->GetVulkanLayout());

        VkDescriptorSetAllocateInfo allocInfo{
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .descriptorPool = _DescriptorPool,
                .descriptorSetCount = count,
                .pSetLayouts = layouts.data()
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        Graphics::CheckVk(vkAllocateDescriptorSets(device, &allocInfo, (VkDescriptorSet*)pSets));
    }

    void GraphicsPipeline::CreateShaderProgram()
    {
        for(auto& shaderStage : m_ShaderStages)
        {
            auto fileLoaded = ReadF(shaderStage);
            _Magma_Assert(fileLoaded.has_value(), "Shader stage could not be loaded");

            auto stageFlag = Shader::GetShaderStage(shaderStage);
            auto shaderModule = m_Shader->CreateShaderModule(shaderStage, fileLoaded.value(), "", stageFlag);

            VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = stageFlag,
                .module = shaderModule,
                .pName = "main"
            };

            m_Modules.emplace_back(shaderModule);
            m_Stages.emplace_back(pipelineShaderStageCreateInfo);
        }

        m_Shader->BuildReflection();
    }

    void GraphicsPipeline::CreateDescriptorLayout()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        const auto& descriptorSetLayouts = m_Shader->GetDescriptorSetLayouts();

        VkDescriptorSetLayoutCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .flags = 0,
            .bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size()),
            .pBindings = descriptorSetLayouts.data()
        };

        Graphics::CheckVk(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &m_DescriptorSetLayout));
    }

    void GraphicsPipeline::CreateDescriptorPool()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        const auto& descriptorPools = m_Shader->GetDescriptorPools();

        VkDescriptorPoolCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = 8192,
            .poolSizeCount = static_cast<uint32_t>(descriptorPools.size()),
            .pPoolSizes = descriptorPools.data()
        };

        Graphics::CheckVk(vkCreateDescriptorPool(device, &createInfo, nullptr, &m_DescriptorPool));
    }

    void GraphicsPipeline::CreatePipelineLayout()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();

        VkPipelineLayoutCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &m_DescriptorSetLayout,
        };

        Graphics::CheckVk(vkCreatePipelineLayout(device, &createInfo, nullptr, &m_PipelineLayout));
    }

    void GraphicsPipeline::CreateAttributes()
    {
        m_InputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_InputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        m_InputAssemblyState.primitiveRestartEnable = false;

        auto extent = Graphics::GetSwapchain()->GetExtent2D();
        m_Scissor.offset = {0, 0};
        m_Scissor.extent = extent;

        m_Viewport.x = 0.0f;
        m_Viewport.y = 0.0f;
        m_Viewport.width = (float)extent.width;
        m_Viewport.height = (float)extent.height;
        m_Viewport.minDepth = 0.0f;
        m_Viewport.maxDepth = 1.0f;

        m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_ViewportState.viewportCount = 1;
        m_ViewportState.pViewports = &m_Viewport;
        m_ViewportState.scissorCount = 1;
        m_ViewportState.pScissors = &m_Scissor;

        m_RasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        m_RasterizationState.depthClampEnable = false;
        m_RasterizationState.rasterizerDiscardEnable = false;
        m_RasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        m_RasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
        m_RasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        m_RasterizationState.depthBiasEnable = false;
        m_RasterizationState.depthBiasConstantFactor = 0.0f;
        m_RasterizationState.depthBiasClamp = 0.0f;
        m_RasterizationState.depthBiasSlopeFactor = 0.0f;
        m_RasterizationState.lineWidth = 1.0f;

        m_MultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_MultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        m_MultisampleState.sampleShadingEnable = false;

        m_ColorBlendAttachmentState.blendEnable = false;
        m_ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        m_ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        m_ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        m_ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        m_ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        m_ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
        m_ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        m_ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_ColorBlendState.logicOpEnable = false;
        m_ColorBlendState.attachmentCount = 1;
        m_ColorBlendState.pAttachments = &m_ColorBlendAttachmentState;
    }

    void GraphicsPipeline::CreatePipeline()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();

        std::vector<VkVertexInputBindingDescription> bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        uint32_t lastAttribute = 0;

        for (const auto& vertexInput : m_VertexInputs)
        {
            for (const auto& binding : vertexInput.GetBindingDescriptions())
                bindingDescriptions.emplace_back(binding);

            for (const auto& attribute : vertexInput.GetAttributeDescriptions())
            {
                auto& attrib = attributeDescriptions.emplace_back(attribute);
                attrib.location += lastAttribute;
            }

            if (!vertexInput.GetAttributeDescriptions().empty())
                lastAttribute = attributeDescriptions.back().location + 1;
        }

        m_VertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_VertexInputState.vertexBindingDescriptionCount= static_cast<uint32_t>(bindingDescriptions.size());
        m_VertexInputState.pVertexBindingDescriptions = bindingDescriptions.data();
        m_VertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        m_VertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkGraphicsPipelineCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.stageCount = static_cast<uint32_t>(m_Stages.size());
        createInfo.pStages = m_Stages.data();

        createInfo.pVertexInputState = &m_VertexInputState;
        createInfo.pInputAssemblyState = &m_InputAssemblyState;
        createInfo.pViewportState = &m_ViewportState;
        createInfo.pRasterizationState = &m_RasterizationState;
        createInfo.pMultisampleState = &m_MultisampleState;
        //createInfo.pDepthStencilState = &m_DepthStencilState;
        createInfo.pColorBlendState = &m_ColorBlendState;

        createInfo.layout = m_PipelineLayout;
        createInfo.renderPass = m_Renderpass;

        Graphics::CheckVk(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_Pipeline));
    }

}
