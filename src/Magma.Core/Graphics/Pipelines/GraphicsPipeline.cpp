//
// Created by Acer on 12.11.2021 г..
//

#include "Magma.Core/Graphics/Pipelines/GraphicsPipeline.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Engine/Log.h"

#include <array>

namespace Magma
{
    GraphicsPipeline::GraphicsPipeline(const std::vector<std::filesystem::path>& shaderPaths, VkDescriptorSetLayout descriptorSetLayout)
        : _Layout(VK_NULL_HANDLE), _Pipeline(VK_NULL_HANDLE)
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto renderpass = Graphics::GetRenderpass()->GetVulkanRenderpass();
        auto swapchain = Graphics::GetSwapchain();

        auto bindingDescription = Vertex::GetBinding();
        auto attributeDescriptions = Vertex::GetAttributes();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
          .vertexBindingDescriptionCount = 1,
          .pVertexBindingDescriptions = &bindingDescription,
          .vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size(),
          .pVertexAttributeDescriptions = attributeDescriptions.data()
        };

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };

        VkExtent2D extent = swapchain->GetExtent2D();
        VkViewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)extent.width,
            .height = (float)extent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        VkRect2D scissor{
            .offset = {0, 0},
            .extent = extent
        };

        VkPipelineViewportStateCreateInfo viewportStateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor
        };

        VkPipelineRasterizationStateCreateInfo rasterizerInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = false,
            .rasterizerDiscardEnable = false,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f,
            .lineWidth = 1.0f,
        };

        VkPipelineMultisampleStateCreateInfo multisamplingInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = false,
        };

        //TODO:
        VkPipelineColorBlendAttachmentState colorBlendAttachment{
            .blendEnable = false,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        VkPipelineColorBlendStateCreateInfo colorBlendingInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = false,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment
        };

        std::array<VkDynamicState, 2> dynamicStates{
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicStateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = dynamicStates.size(),
            .pDynamicStates = dynamicStates.data()
        };

        VkPipelineLayoutCreateInfo layoutInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout
        };


        auto result = vkCreatePipelineLayout(device, &layoutInfo, nullptr, &_Layout);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not create pipeline layout"));

        std::vector<Shader> shaders;
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
        for(const auto& path : shaderPaths)
            shaders.emplace_back(path);
        CopyShaderStageCreateInfo(shaders, shaderStages);

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportStateInfo;
        pipelineInfo.pRasterizationState = &rasterizerInfo;
        pipelineInfo.pMultisampleState = &multisamplingInfo;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlendingInfo;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = _Layout;
        pipelineInfo.renderPass = renderpass;
        pipelineInfo.subpass = 0;

        result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_Pipeline);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not create graphics pipeline"));
        _Magma_Core_Info("Created graphics pipeline");
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkDestroyPipeline(device, _Pipeline, nullptr);
        vkDestroyPipelineLayout(device, _Layout, nullptr);
    }

}
