//
// Created by Acer on 30.11.2021 г..
//

#ifndef MAGMA_DESCRIPTOR_SET_H
#define MAGMA_DESCRIPTOR_SET_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Graphics/Pipelines/Shader.h"

#include <string>
#include <initializer_list>
#include <utility>
#include <vulkan/vulkan.h>

namespace Magma
{
    enum class DescriptorType : uint8_t
    {
        UniformBuffer
    };

    class DescriptorSetBinding
    {
    public:
        DescriptorSetBinding(std::string&& name, ShaderStage stage, DescriptorType type)
            : Name(std::move(name)), Stage(stage), Type(type)
        {

        }

        DescriptorType Type;
        ShaderStage Stage;
        std::string Name;
    };

    class MAGMA_API DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::initializer_list<DescriptorSetBinding>& bindings);
        ~DescriptorSetLayout() noexcept;

        [[nodiscard]] const VkDescriptorSetLayout& GetVulkanLayout() const noexcept { return _Layout; }
    private:
        VkDescriptorSetLayout _Layout;
    };

    class UniformBuffer;

    class MAGMA_API DescriptorSet
    {
    public:
        DescriptorSet() = default;

        void Update(uint32_t size, const Ref<UniformBuffer>& buffer);

        [[nodiscard]] const VkDescriptorSet& GetVulkanDescriptorSet() const noexcept { return _DescriptorSet; }
    private:
        VkDescriptorSet _DescriptorSet;
    };
}

#endif //MAGMA_DESCRIPTOR_SET_H
