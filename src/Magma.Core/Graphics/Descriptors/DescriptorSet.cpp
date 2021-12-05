//
// Created by Acer on 30.11.2021 г..
//

#include "Magma.Core/Graphics/Descriptors/DescriptorSet.h"
#include "Magma.Core/Graphics/Graphics.h"

#include "Magma.Core/Graphics/Buffers/UniformBuffer.h"

#include <vector>

namespace Magma
{
    constexpr VkDescriptorType DescriptorTypeToVk(DescriptorType type)
    {
        switch (type)
        {
            case DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }

        return (VkDescriptorType)0;
    }


    DescriptorSetLayout::DescriptorSetLayout(const std::initializer_list<DescriptorSetBinding> &bindings)
        : _Layout(VK_NULL_HANDLE)
    {
        uint32_t size = bindings.size();
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings(size);

        int i = 0;
        for(auto& binding : bindings)
        {
            layoutBindings[i].binding = i;
            layoutBindings[i].descriptorType = DescriptorTypeToVk(binding.Type);
            layoutBindings[i].descriptorCount = 1;
            layoutBindings[i].stageFlags = ShaderTypeToVkShaderStage(binding.Stage);
            ++i;
        }

        VkDescriptorSetLayoutCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = size,
            .pBindings = layoutBindings.data()
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        Graphics::CheckVk(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &_Layout));
    }

    void DescriptorSet::Update(uint32_t size, const Ref<UniformBuffer>& buffer)
    {
        VkDescriptorBufferInfo bufferInfo{
                .buffer = buffer->GetBuffer(),
                .offset = 0,
                .range = size
        };

        VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = _DescriptorSet,
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo = &bufferInfo,
        };

        auto device = Graphics::GetDevice()->GetVulkanDevice();
        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }

    DescriptorSetLayout::~DescriptorSetLayout() noexcept
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        //vkDestroyDescriptorSetLayout(device, _Layout, nullptr);
    }
}


