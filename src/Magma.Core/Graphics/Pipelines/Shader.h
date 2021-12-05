//
// Created by Acer on 11.11.2021 г..
//

#ifndef MAGMA_SHADER_H
#define MAGMA_SHADER_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Devices/LogicalDevice.h"

#include <vulkan/vulkan.h>
#include <glslang/Public/ShaderLang.h>

#include <string>
#include <map>
#include <vector>
#include <filesystem>



namespace Magma
{
    enum class ShaderStage : uint8_t
    {
        Vertex = 0,
        Tesselation,
        Geometry,
        Fragment,
        Compute,
        All
    };

    constexpr const char* ShaderTypeToString(ShaderStage type)
    {
        switch (type)
        {
            case ShaderStage::Vertex: return "Vertex";
            case ShaderStage::Tesselation: return "Tesselation";
            case ShaderStage::Geometry: return "Geometry";
            case ShaderStage::Fragment: return "Fragment";
            case ShaderStage::Compute: return "Compute";
            case ShaderStage::All: return "All";
        }

        return nullptr;
    }

    constexpr VkShaderStageFlagBits ShaderTypeToVkShaderStage(ShaderStage type)
    {
        switch (type)
        {
            case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
            case ShaderStage::Tesselation: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; //TODO:
            case ShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
            case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
            case ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
            case ShaderStage::All: return VK_SHADER_STAGE_ALL_GRAPHICS;
        }

        return (VkShaderStageFlagBits)0;
    }

    class MAGMA_API Shader
    {
    public:
        class VertexInput
        {
            friend class Shader;
        public:
            VertexInput(std::vector<VkVertexInputBindingDescription> bindingDescriptions = {}, std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {}) :
                    bindingDescriptions(std::move(bindingDescriptions)),
                    attributeDescriptions(std::move(attributeDescriptions)) {
            }

            [[nodiscard]] const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return bindingDescriptions; }
            [[nodiscard]] const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return attributeDescriptions; }

            bool operator<(const VertexInput &rhs) const {
                return bindingDescriptions.front().binding < rhs.bindingDescriptions.front().binding;
            }
        private:
            uint32_t binding = 0;
            std::vector<VkVertexInputBindingDescription> bindingDescriptions;
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        };

        class Uniform
                {
            friend class Shader;
        public:
            explicit Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1, int32_t glType = -1, bool readOnly = false,
                             bool writeOnly = false, VkShaderStageFlags stageFlags = 0) :
                    binding(binding),
                    offset(offset),
                    size(size),
                    glType(glType),
                    readOnly(readOnly),
                    writeOnly(writeOnly),
                    stageFlags(stageFlags) {
            }

            int32_t GetBinding() const { return binding; }
            int32_t GetOffset() const { return offset; }
            int32_t GetSize() const { return size; }
            int32_t GetGlType() const { return glType; }
            bool IsReadOnly() const { return readOnly; }
            bool IsWriteOnly() const { return writeOnly; }
            VkShaderStageFlags GetStageFlags() const { return stageFlags; }

            bool operator==(const Uniform &rhs) const
            {
                return binding == rhs.binding && offset == rhs.offset && size == rhs.size && glType == rhs.glType && readOnly == rhs.readOnly &&
                       writeOnly == rhs.writeOnly && stageFlags == rhs.stageFlags;
            }

            bool operator!=(const Uniform &rhs) const
            {
                return !operator==(rhs);
            }
        private:
            int32_t binding;
            int32_t offset;
            int32_t size;
            int32_t glType;
            bool readOnly;
            bool writeOnly;
            VkShaderStageFlags stageFlags;
        };

        class UniformBlock {
            friend class Shader;
        public:
            enum class Type { None, Uniform, Storage, Push };

            explicit UniformBlock(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0, Type type = Type::Uniform) :
                    binding(binding),
                    size(size),
                    stageFlags(stageFlags),
                    type(type) {
            }

            int32_t GetBinding() const { return binding; }
            int32_t GetSize() const { return size; }
            VkShaderStageFlags GetStageFlags() const { return stageFlags; }
            Type GetType() const { return type; }
            const std::map<std::string, Uniform> &GetUniforms() const { return uniforms; }

            std::optional<Uniform> GetUniform(const std::string &name) const {
                auto it = uniforms.find(name);

                if (it == uniforms.end()) {
                    return std::nullopt;
                }

                return it->second;
            }

            bool operator==(const UniformBlock &rhs) const {
                return binding == rhs.binding && size == rhs.size && stageFlags == rhs.stageFlags && type == rhs.type && uniforms == rhs.uniforms;
            }

            bool operator!=(const UniformBlock &rhs) const {
                return !operator==(rhs);
            }

        private:
            int32_t binding;
            int32_t size;
            VkShaderStageFlags stageFlags;
            Type type;
            std::map<std::string, Uniform> uniforms;
        };

        class Attribute {
            friend class Shader;
        public:
            explicit Attribute(int32_t set = -1, int32_t location = -1, int32_t size = -1, int32_t glType = -1) :
                    set(set),
                    location(location),
                    size(size),
                    glType(glType) {
            }

            int32_t GetSet() const { return set; }
            int32_t GetLocation() const { return location; }
            int32_t GetSize() const { return size; }
            int32_t GetGlType() const { return glType; }

            bool operator==(const Attribute &rhs) const {
                return set == rhs.set && location == rhs.location && size == rhs.size && glType == rhs.glType;
            }

            bool operator!=(const Attribute &rhs) const {
                return !operator==(rhs);
            }
        private:
            int32_t set;
            int32_t location;
            int32_t size;
            int32_t glType;
        };

        static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& file);

        Shader();
        ~Shader();

        VkShaderModule CreateShaderModule(const std::filesystem::path& file, const std::string &moduleCode, const std::string &preamble, VkShaderStageFlagBits stageFlags);

    private:
        void LoadUniformBlock(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i);
        void LoadUniform(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i);
        void LoadAttribute(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i);
        int32_t ComputeSize(const glslang::TType *ttype);
    private:
        std::vector<std::filesystem::path> m_Stages;

        std::map<std::string, Uniform> m_Uniforms;
        std::map<std::string, UniformBlock> m_UniformBlocks;
        std::map<std::string, Attribute> m_Attributes;

        std::array<std::optional<uint32_t>, 3> m_LocalSizes;

    };

    void MAGMA_API CopyShaderStageCreateInfo(const std::vector<Shader>& src, std::vector<VkPipelineShaderStageCreateInfo>& dst);
}

#endif //MAGMA_SHADER_H
