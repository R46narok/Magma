//
// Created by Acer on 16.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Graphics/Renderer2D.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Graphics/Descriptors/DescriptorSet.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Magma
{
    struct Vertex2D
    {
        glm::vec3 Position;
        glm::vec3 Color;

        static VkVertexInputBindingDescription GetBinding()
        {
            return {
                    .binding = 0,
                    .stride = sizeof(Vertex2D),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
        }

        static std::vector<VkVertexInputAttributeDescription> GetAttributes()
        {
            std::vector<VkVertexInputAttributeDescription> attribs(2);

            attribs[0].binding = 0;
            attribs[0].location = 0;
            attribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribs[0].offset = offsetof(Vertex2D, Position);

            attribs[1].binding = 0;
            attribs[1].location = 1;
            attribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribs[1].offset = offsetof(Vertex2D, Color);

            return attribs;
        }
    };


    const std::vector<Vertex2D> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f,  0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };

    static constexpr uint32_t MaxFramesInFlight = 2;

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    VkDescriptorPool descriptorPool;
    std::vector<DescriptorSet> descriptorSets;

    std::vector<Ref<UniformBuffer>> UniformBuffers;

    Ref<DescriptorSetLayout> layout;
    Renderer2D::Renderer2D()
    {
        _Renderpass = CreateRef<Renderpass>();
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        layout.reset(new DescriptorSetLayout({{"", ShaderStage::Vertex, DescriptorType::UniformBuffer}}));

        auto imageCount = Graphics::GetSwapchain()->GetImageCount();

        for(int i = 0; i < imageCount; ++i)
        {
            UniformBuffers.push_back(CreateRef<UniformBuffer>(sizeof(UniformBufferObject)));
        }

        /*Shader vertex("vert.spv", ShaderStage::Vertex);
        Shader fragment("frag.spv", ShaderStage::Fragment);
        vertex.PushUniform({"UniformBufferObject", 0, 0, sizeof(UniformBufferObject), ShaderStage::Vertex});*/

        std::vector<std::filesystem::path> shaders{"shader.vert", "shader.frag"};
        std::vector<Shader::VertexInput> input {Shader::VertexInput({Vertex2D::GetBinding()}, Vertex2D::GetAttributes())};
        _Pipeline = CreateRef<GraphicsPipeline>(shaders, _Renderpass, input);

        descriptorSets.resize(imageCount);
        _Pipeline->AllocateDescriptorSets(descriptorSets.data(), layout);

        for(int i = 0; i < imageCount; ++i)
            descriptorSets[i].Update(sizeof(UniformBufferObject), UniformBuffers[i]);

        _Framebuffers = CreateRef<Framebuffers>(_Renderpass);

        auto size = sizeof(vertices[0]) * vertices.size();
        _VertexBuffer = CreateRef<Buffer>(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* pData = _VertexBuffer->MapMemory();
        std::memcpy(pData, vertices.data(), size);
        _VertexBuffer->UnmapMemory();

        size = sizeof(indices[0]) * indices.size();
        _IndexBuffer = CreateRef<Buffer>(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        pData = stagingBuffer.MapMemory();
        std::memcpy(pData, indices.data(), size);
        stagingBuffer.UnmapMemory();

        Buffer::Copy(stagingBuffer.GetBuffer(), _IndexBuffer->GetBuffer(), size);

        _CommandBuffers.resize(imageCount);
        for (int i = 0; i < imageCount; ++i)
        {
            auto cmd = new (&_CommandBuffers[i]) CommandBuffer();

            cmd->Begin();
            cmd->BeginRenderpass(_Renderpass, (*_Framebuffers)[i], {.color{0.0f, 0.0f, 0.0f, 1.0f}});
            cmd->BindPipeline(_Pipeline);

            VkDeviceSize offsets[] = {0};
            cmd->BindVertexBuffer(_VertexBuffer, 0);
            cmd->BindIndexBuffer(_IndexBuffer, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(cmd->GetCommandBuffer(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, _Pipeline->GetPipelineLayout(), 0, 1, &descriptorSets[i].GetVulkanDescriptorSet(), 0, nullptr);
            cmd->DrawIndexed(indices.size());
            cmd->DrawIndexed(indices.size());
            cmd->EndRenderpass();

            cmd->End();
        }

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        _ImageAvailableSemaphore.resize(MaxFramesInFlight);
        _RenderFinishedSemaphore.resize(MaxFramesInFlight);
        _InFlightFences.resize(MaxFramesInFlight);
        _ImagesInFlight.resize(imageCount, VK_NULL_HANDLE);


        for (int i = 0; i < MaxFramesInFlight; ++i)
        {
            Graphics::CheckVk(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_ImageAvailableSemaphore[i]));
            Graphics::CheckVk( vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_RenderFinishedSemaphore[i]));

            Graphics::CheckVk(vkCreateFence(device, &fenceInfo, nullptr, &_InFlightFences[i]));
        }
    }

    Renderer2D::~Renderer2D()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();

        vkDeviceWaitIdle(device);

        for(int i = 0; i < MaxFramesInFlight; ++i)
        {
            vkDestroySemaphore(device, _ImageAvailableSemaphore[i], nullptr);
            vkDestroySemaphore(device, _RenderFinishedSemaphore[i], nullptr);
            vkDestroyFence(device, _InFlightFences[i], nullptr);
        }

        UniformBuffers.clear();
        _VertexBuffer.reset();
        _IndexBuffer.reset();
        _Pipeline.reset();
    }

    static uint32_t currentFrame = 0;
    void Renderer2D::Update()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();

        vkWaitForFences(device, 1, &_InFlightFences[currentFrame], true, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device, Graphics::GetSwapchain()->GetSwapchain(), UINT64_MAX, _ImageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(device, 1, &_ImagesInFlight[imageIndex], true, UINT64_MAX);

        _ImagesInFlight[imageIndex] = _InFlightFences[currentFrame];

        UpdateUniformBuffer(imageIndex);

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSubmitInfo submitInfo{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &_ImageAvailableSemaphore[currentFrame],
                .pWaitDstStageMask = waitStages,
                .commandBufferCount = 1,
                .pCommandBuffers = &(_CommandBuffers[imageIndex].GetCommandBuffer()),
                .signalSemaphoreCount = 1,
                .pSignalSemaphores = &_RenderFinishedSemaphore[currentFrame]
        };

        vkResetFences(device, 1, &_InFlightFences[currentFrame]);
        auto result = vkQueueSubmit(Graphics::GetDevice()->GetGraphicsQueue(), 1, &submitInfo, _InFlightFences[currentFrame]);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not submit draw cmd buffer"));

        VkPresentInfoKHR presentInfo{
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &_RenderFinishedSemaphore[currentFrame],
                .swapchainCount = 1,
                .pSwapchains = &Graphics::GetSwapchain()->GetSwapchain(),
                .pImageIndices = &imageIndex
        };

        vkQueuePresentKHR(Graphics::GetDevice()->GetPresentQueue(), &presentInfo);
        currentFrame = (currentFrame + 1) % MaxFramesInFlight;
    }

    void Renderer2D::UpdateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        auto extent = Graphics::GetSwapchain()->GetExtent2D();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);

        ubo.proj[1][1] *= -1;

        auto pData = UniformBuffers[currentImage]->MapMemory();
        std::memcpy(pData, &ubo, sizeof(ubo));
        UniformBuffers[currentImage]->UnmapMemory();
    }
}



