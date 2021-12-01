//
// Created by Acer on 11.11.2021 г..
//

#include "Magma.Core/Graphics/Pipelines/Shader.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Filesystem/File.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{

    Shader::Shader(const std::filesystem::path& path)
        : _Module(VK_NULL_HANDLE), _StageCreateInfo({})
    {
        auto filename = path.filename();
        if (filename == "vert.spv") _Type = ShaderType::Vertex;
        if (filename == "frag.spv") _Type = ShaderType::Fragment ;

        auto bytes = ReadFileBinary(path);
        if (bytes.has_value())
        {
            VkShaderModuleCreateInfo createInfo{
              .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
              .codeSize = bytes.value().size(),
              .pCode = reinterpret_cast<const uint32_t*>(bytes.value().data())
            };

            auto device = Graphics::GetDevice()->GetVulkanDevice();
            auto result = vkCreateShaderModule(device, &createInfo, nullptr, &_Module);
            _Magma_Assert(result == VK_SUCCESS, _Magma_Core_Error("Could not create VkShaderModule of type {0}", ShaderTypeToString(_Type)));

            _StageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            _StageCreateInfo.stage = ShaderTypeToVkShaderStage(_Type);
            _StageCreateInfo.module = _Module;
            _StageCreateInfo.pName = "main";
        }
    }

    Shader::~Shader()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        //vkDestroyShaderModule(device, _Module, nullptr);
    }

    void CopyShaderStageCreateInfo(const std::vector<Shader>& src, std::vector<VkPipelineShaderStageCreateInfo>& dst)
    {
        dst.resize(src.size());
        for (int i = 0; i < src.size(); ++i)
            dst[i] = src[i].GetStageCreateInfo();
    }
}
