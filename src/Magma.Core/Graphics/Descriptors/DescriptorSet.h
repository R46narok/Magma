//
// Created by Acer on 30.11.2021 г..
//

#ifndef MAGMA_DESCRIPTOR_SET_H
#define MAGMA_DESCRIPTOR_SET_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <string>
#include <initializer_list>
#include <vulkan/vulkan.h>

namespace Magma
{
    enum class ShaderStage: uint8_t
    {
        Vertex = 0,
        Tesselation,
        Geometry,
        Fragment,
        Compute,
        All
    };

    enum class DescriptorType : uint8_t
    {
        UniformBuffer
    };

    class DescriptorSetBinding
    {
    public:
        DescriptorSetBinding(const std::string& name, ShaderStage stage, DescriptorType type)
            : Name(name), Stage(stage), Type(type)
        {

        }

        DescriptorType Type;
        ShaderStage Stage;
        std::string Name;
    };

    class _Magma_Dll DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::initializer_list<DescriptorSetBinding>& bindings);
        ~DescriptorSetLayout() noexcept;

        [[nodiscard]] const VkDescriptorSetLayout& GetVulkanLayout() const noexcept { return _Layout; }
    private:
        VkDescriptorSetLayout _Layout;
    };

    class UniformBuffer;

    class _Magma_Dll DescriptorSet
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
