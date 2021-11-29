//
// Created by Acer on 11.11.2021 г..
//

#ifndef MAGMA_SHADER_H
#define MAGMA_SHADER_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/LogicalDevice.h"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <filesystem>

namespace Magma
{
    enum class ShaderType : uint8_t
    {
        Vertex = 0,
        Tesselation,
        Geometry,
        Fragment,
        Compute
    };

    constexpr const char* ShaderTypeToString(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex: return "Vertex";
            case ShaderType::Tesselation: return "Tesselation";
            case ShaderType::Geometry: return "Geometry";
            case ShaderType::Fragment: return "Fragment";
            case ShaderType::Compute: return "Compute";
        }

        return nullptr;
    }

    constexpr VkShaderStageFlagBits ShaderTypeToVkShaderStage(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
            case ShaderType::Tesselation: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; //TODO:
            case ShaderType::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
            case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
            case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
        }

        return (VkShaderStageFlagBits)0;
    }

    class _Magma_Dll Shader
    {
    public:
        explicit Shader(const std::filesystem::path& path);
        ~Shader();

        [[nodiscard]] const VkPipelineShaderStageCreateInfo& GetStageCreateInfo() const noexcept { return _StageCreateInfo; }
    private:
        VkShaderModule _Module;
        VkPipelineShaderStageCreateInfo _StageCreateInfo;

        ShaderType _Type;
    };

    void _Magma_Dll CopyShaderStageCreateInfo(const std::vector<Shader>& src, std::vector<VkPipelineShaderStageCreateInfo>& dst);
}

#endif //MAGMA_SHADER_H
