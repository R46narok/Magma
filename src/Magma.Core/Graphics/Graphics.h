//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_GRAPHICS_H
#define MAGMA_GRAPHICS_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Engine/Module.h"
#include "Magma.Core/Devices/Instance.h"
#include "Magma.Core/Devices/Surface.h"
#include "Magma.Core/Devices/PhysicalDevice.h"
#include "Magma.Core/Devices/LogicalDevice.h"
#include "Magma.Core/Graphics/Renderpass/Renderpass.h"
#include "Magma.Core/Graphics/Pipelines/GraphicsPipeline.h"
#include "Magma.Core/Graphics/Renderpass/Swapchain.h"
#include "Magma.Core/Graphics/Renderpass/Framebuffer.h"
#include "Magma.Core/Graphics/Commands/CommandBuffer.h"
#include "Magma.Core/Graphics/Commands/CommandPool.h"
#include "Magma.Core/Graphics/Buffers/Buffer.h"
#include "Magma.Core/Window.h"

#include <vector>
#include <glm/glm.hpp>

namespace Magma
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Color;

        static VkVertexInputBindingDescription GetBinding()
        {
            return {
                    .binding = 0,
                    .stride = sizeof(Vertex),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
        }

        static std::array<VkVertexInputAttributeDescription, 2> GetAttributes()
        {
            std::array<VkVertexInputAttributeDescription, 2> attribs{};

            attribs[0].binding = 0;
            attribs[0].location = 0;
            attribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribs[0].offset = offsetof(Vertex, Position);

            attribs[1].binding = 0;
            attribs[1].location = 1;
            attribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attribs[1].offset = offsetof(Vertex, Color);

            return attribs;
        }
    };



    class _Magma_Dll Graphics : public Module::Registrar<Graphics>
    {
    private:
        [[maybe_unused]] inline static const bool Registered = Register(Stage::Render, Requires<Window>());
    public:
        Graphics();
        ~Graphics() noexcept override;

        void Update() override;

        static void CheckVk(VkResult result);

        static const Instance* GetInstance() noexcept { return moduleInstance->_Instance.get(); }
        static const Surface* GetSurface() noexcept { return moduleInstance->_Surface.get(); }
        static const CommandPool* GetCommandPool() noexcept { return moduleInstance->_Pool.get(); }
        static const PhysicalDevice* GetPhysicalDevice() noexcept { return moduleInstance->_PhysicalDevice.get(); }
        static const LogicalDevice* GetDevice() noexcept { return moduleInstance->_LogicalDevice.get(); }
        static const Swapchain* GetSwapchain() noexcept { return moduleInstance->_Swapchain.get(); }
        static const Renderpass* GetRenderpass() noexcept { return moduleInstance->_Renderpass.get(); }
    private:

        void UpdateUniformBuffer(uint32_t currentImage);

        Ref<Instance> _Instance;
        Ref<Surface> _Surface;
        Ref<PhysicalDevice> _PhysicalDevice;
        Ref<LogicalDevice> _LogicalDevice;
        Ref<Swapchain> _Swapchain;

        Ref<Framebuffers> _Framebuffers;
        Ref<Renderpass> _Renderpass;
        Ref<GraphicsPipeline> _Pipeline;

        Ref<Buffer> _VertexBuffer;
        Ref<Buffer> _IndexBuffer;

        std::vector<VkSemaphore> ImageAvailableSemaphore;
        std::vector<VkSemaphore> RenderFinishedSemaphore;
        std::vector<VkFence> InFlightFences;
        std::vector<VkFence> ImagesInFlight;

        Ref<CommandPool> _Pool;
        std::vector<CommandBuffer> _CommandBuffers;
    };
}

#endif //MAGMA_GRAPHICS_H
