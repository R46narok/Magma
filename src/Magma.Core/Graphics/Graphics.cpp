//
// Created by Acer on 12.11.2021 г..
//

#include <Magma.Core/Engine/Log.h>
#include "Magma.Core/Graphics/Graphics.h"

#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace Magma
{
    const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<Scope<Buffer>> UniformBuffers;

    Graphics::Graphics()
    {
        moduleInstance = this;
        _Instance = CreateRef<Instance>();
        _Surface = CreateRef<Surface>();
        _PhysicalDevice = CreateRef<PhysicalDevice>();
        _LogicalDevice = CreateRef<LogicalDevice>();
        _Swapchain = CreateRef<Swapchain>();
        _Renderpass = CreateRef<Renderpass>();

        VkDescriptorSetLayoutBinding uboLayoutBinding{
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT
        };

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = 1,
            .pBindings = &uboLayoutBinding
        };

        auto result = vkCreateDescriptorSetLayout(_LogicalDevice->GetVulkanDevice(), &layoutCreateInfo, nullptr, &descriptorSetLayout);

        std::vector<std::filesystem::path> shaders{"vert.spv", "frag.spv"};
        _Pipeline = CreateRef<GraphicsPipeline>(shaders, descriptorSetLayout);
        _Framebuffers = CreateRef<Framebuffers>();
        _Pool = CreateRef<CommandPool>(_PhysicalDevice, _LogicalDevice, _Surface);

        {
            auto size = sizeof(vertices[0]) * vertices.size();
            _VertexBuffer = CreateRef<Buffer>(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            auto* pData = stagingBuffer.MapMemory();
            std::memcpy(pData, vertices.data(), size);
            stagingBuffer.UnmapMemory();

            Buffer::Copy(stagingBuffer.GetBuffer(), _VertexBuffer->GetBuffer(), size);
        }


        auto size = sizeof(indices[0]) * indices.size();
        _IndexBuffer = CreateRef<Buffer>(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        auto* pData = stagingBuffer.MapMemory();
        std::memcpy(pData, indices.data(), size);
        stagingBuffer.UnmapMemory();

        Buffer::Copy(stagingBuffer.GetBuffer(), _IndexBuffer->GetBuffer(), size);

        auto framebuffers = _Framebuffers->GetFramebuffers();

        for(int i = 0; i < framebuffers.size(); ++i)
        {
            UniformBuffers.push_back(CreateScope<Buffer>(sizeof(UniformBufferObject),
                                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        }

        VkDescriptorPoolSize poolSize{
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = (uint32_t)framebuffers.size()
        };

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = (uint32_t)framebuffers.size(),
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize,
        };

        result = vkCreateDescriptorPool(_LogicalDevice->GetVulkanDevice(), &descriptorPoolCreateInfo, nullptr, &descriptorPool);

        std::vector<VkDescriptorSetLayout> layouts(framebuffers.size(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = descriptorPool,
            .descriptorSetCount = (uint32_t)framebuffers.size(),
            .pSetLayouts = layouts.data()
        };

        descriptorSets.resize(framebuffers.size());
        vkAllocateDescriptorSets(_LogicalDevice->GetVulkanDevice(), &allocInfo, descriptorSets.data());

        for(int i = 0; i < framebuffers.size(); ++i)
        {
            VkDescriptorBufferInfo bufferInfo{
                .buffer = UniformBuffers[i]->GetBuffer(),
                .offset = 0,
                .range = sizeof(UniformBufferObject)
            };

            VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = descriptorSets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo = &bufferInfo,
            };

            vkUpdateDescriptorSets(_LogicalDevice->GetVulkanDevice(), 1, &descriptorWrite, 0, nullptr);
        }

        _CommandBuffers.resize(framebuffers.size());
        for (int i = 0; i < framebuffers.size(); ++i)
        {
            auto cmd = new (&_CommandBuffers[i]) CommandBuffer();

            cmd->Begin();
            cmd->BeginRenderpass(_Renderpass, framebuffers[i]);
            cmd->BindPipeline(_Pipeline);

            VkDeviceSize offsets[] = {0};
            cmd->BindVertexBuffer(_VertexBuffer, offsets);
            cmd->BindIndexBuffer(_IndexBuffer, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(cmd->GetCommandBuffer(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, _Pipeline->GetPipelineLayout(), 0, 1, &descriptorSets[i], 0, nullptr);
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

        ImageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
        RenderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
        InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        ImagesInFlight.resize(framebuffers.size(), VK_NULL_HANDLE);

        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            result = vkCreateSemaphore(_LogicalDevice->GetVulkanDevice(), &semaphoreInfo, nullptr, &ImageAvailableSemaphore[i]);
            _Magma_VkAssert(result, _Magma_Core_Error("Could not create semaphore"));
            result = vkCreateSemaphore(_LogicalDevice->GetVulkanDevice(), &semaphoreInfo, nullptr, &RenderFinishedSemaphore[i]);
            _Magma_VkAssert(result, _Magma_Core_Error("Could not create semaphore"));

            result = vkCreateFence(_LogicalDevice->GetVulkanDevice(), &fenceInfo, nullptr, &InFlightFences[i]);
            _Magma_VkAssert(result, _Magma_Core_Error("Could not create fence"));
        }
    }

    Graphics::~Graphics() noexcept
    {
        vkDeviceWaitIdle(_LogicalDevice->_Device);
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            vkDestroySemaphore(_LogicalDevice->GetVulkanDevice(), ImageAvailableSemaphore[i], nullptr);
            vkDestroySemaphore(_LogicalDevice->GetVulkanDevice(), RenderFinishedSemaphore[i], nullptr);
            vkDestroyFence(_LogicalDevice->GetVulkanDevice(), InFlightFences[i], nullptr);
        }

        _VertexBuffer.reset();
        _Framebuffers.reset();
        _Pool.reset();
        vkDestroyDescriptorSetLayout(_LogicalDevice->GetVulkanDevice(), descriptorSetLayout, nullptr);
        UniformBuffers.clear();
        vkDestroyDescriptorPool(_LogicalDevice->GetVulkanDevice(), descriptorPool, nullptr);
        _Pipeline.reset();
        _Swapchain.reset();
        _LogicalDevice.reset();
        _Surface.reset();
        _Instance.reset();
    }

    uint32_t currentFrame = 0;

    void Graphics::Update()
    {
        vkWaitForFences(_LogicalDevice->GetVulkanDevice(), 1, &InFlightFences[currentFrame], true, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(_LogicalDevice->GetVulkanDevice(), _Swapchain->GetSwapchain(), UINT64_MAX, ImageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(_LogicalDevice->GetVulkanDevice(), 1, &ImagesInFlight[imageIndex], true, UINT64_MAX);

        ImagesInFlight[imageIndex] = InFlightFences[currentFrame];

        UpdateUniformBuffer(imageIndex);

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &ImageAvailableSemaphore[currentFrame],
            .pWaitDstStageMask = waitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &(_CommandBuffers[imageIndex].GetCommandBuffer()),
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &RenderFinishedSemaphore[currentFrame]
        };

        vkResetFences(_LogicalDevice->GetVulkanDevice(), 1, &InFlightFences[currentFrame]);
        auto result = vkQueueSubmit(_LogicalDevice->_GraphicsQueue, 1, &submitInfo, InFlightFences[currentFrame]);
        _Magma_VkAssert(result, _Magma_Core_Error("Could not submit draw cmd buffer"));

        VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &RenderFinishedSemaphore[currentFrame],
            .swapchainCount = 1,
            .pSwapchains = &_Swapchain->GetSwapchain(),
            .pImageIndices = &imageIndex
        };

        vkQueuePresentKHR(_LogicalDevice->_PresentQueue, &presentInfo);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Graphics::UpdateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        auto extent = _Swapchain->GetExtent2D();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);

        ubo.proj[1][1] *= -1;

        auto pData = UniformBuffers[currentImage]->MapMemory();
        std::memcpy(pData, &ubo, sizeof(ubo));
        UniformBuffers[currentImage]->UnmapMemory();
    }

    void Graphics::CheckVk(VkResult result)
    {

    }
}


