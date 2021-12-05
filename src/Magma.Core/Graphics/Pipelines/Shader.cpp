//
// Created by Acer on 11.11.2021 г..
//

#include "Magma.Core/Graphics/Pipelines/Shader.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Filesystem/File.h"
#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Utils/String.h"

#include <glslang/SPIRV/GlslangToSpv.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <../../../submodules/physfs/src/physfs.h>

namespace Magma
{

    std::optional<std::string> ReadFF(const std::filesystem::path &path)
    {
        std::ifstream file(path);
        std::string content((std::istreambuf_iterator<char>(file)),
                             (std::istreambuf_iterator<char>()));

        return content;
    }

    class ShaderIncluder : public glslang::TShader::Includer
            {
    public:
        IncludeResult *includeLocal(const char *headerName, const char *includerName, size_t inclusionDepth) override {
            auto directory = std::filesystem::path(includerName).parent_path();
            auto fileLoaded = ReadFF(directory / headerName);

            auto content = new char[fileLoaded->size()];
            std::memcpy(content, fileLoaded->c_str(), fileLoaded->size());
            return new IncludeResult(headerName, content, fileLoaded->size(), content);
        }

        IncludeResult *includeSystem(const char *headerName, const char *includerName, size_t inclusionDepth) override {
            auto fileLoaded = ReadFF(headerName);

            auto content = new char[fileLoaded->size() ];
            std::memcpy(content, fileLoaded->c_str(), fileLoaded->size() );
            return new IncludeResult(headerName, content, fileLoaded->size() , content);
        }

        void releaseInclude(IncludeResult *result) override {
            if (result) {
                delete[] static_cast<char *>(result->userData);
                delete result;
            }
        }
    };

    Shader::Shader()
    {
    }

    Shader::~Shader()
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        //vkDestroyShaderModule(device, _Module, nullptr);
    }

    VkShaderStageFlagBits Shader::GetShaderStage(const std::filesystem::path &file)
    {
        auto ext = file.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == ".comp") return VK_SHADER_STAGE_COMPUTE_BIT;
        if (ext == ".vert") return VK_SHADER_STAGE_VERTEX_BIT;
        if (ext == ".tesc") return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if (ext == ".tese") return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if (ext == ".geom") return VK_SHADER_STAGE_GEOMETRY_BIT;
        if (ext == ".frag") return VK_SHADER_STAGE_FRAGMENT_BIT;

        return VK_SHADER_STAGE_ALL;
    }

    constexpr EShLanguage GetEshLanguage(VkShaderStageFlags stageFlag)
    {
        switch (stageFlag)
        {
            case VK_SHADER_STAGE_COMPUTE_BIT: return EShLangCompute;
            case VK_SHADER_STAGE_VERTEX_BIT: return EShLangVertex;
            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShLangTessControl;
            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShLangTessEvaluation;
            case VK_SHADER_STAGE_GEOMETRY_BIT: return EShLangGeometry;
            case VK_SHADER_STAGE_FRAGMENT_BIT: return EShLangFragment;
            default: return EShLangCount;
        }
    }

    TBuiltInResource GetResources() {
        TBuiltInResource resources = {};
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;
        resources.maxClipDistances = 8;
        resources.maxComputeWorkGroupCountX = 65535;
        resources.maxComputeWorkGroupCountY = 65535;
        resources.maxComputeWorkGroupCountZ = 65535;
        resources.maxComputeWorkGroupSizeX = 1024;
        resources.maxComputeWorkGroupSizeY = 1024;
        resources.maxComputeWorkGroupSizeZ = 64;
        resources.maxComputeUniformComponents = 1024;
        resources.maxComputeTextureImageUnits = 16;
        resources.maxComputeImageUniforms = 8;
        resources.maxComputeAtomicCounters = 8;
        resources.maxComputeAtomicCounterBuffers = 1;
        resources.maxVaryingComponents = 60;
        resources.maxVertexOutputComponents = 64;
        resources.maxGeometryInputComponents = 64;
        resources.maxGeometryOutputComponents = 128;
        resources.maxFragmentInputComponents = 128;
        resources.maxImageUnits = 8;
        resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        resources.maxCombinedShaderOutputResources = 8;
        resources.maxImageSamples = 0;
        resources.maxVertexImageUniforms = 0;
        resources.maxTessControlImageUniforms = 0;
        resources.maxTessEvaluationImageUniforms = 0;
        resources.maxGeometryImageUniforms = 0;
        resources.maxFragmentImageUniforms = 8;
        resources.maxCombinedImageUniforms = 8;
        resources.maxGeometryTextureImageUnits = 16;
        resources.maxGeometryOutputVertices = 256;
        resources.maxGeometryTotalOutputComponents = 1024;
        resources.maxGeometryUniformComponents = 1024;
        resources.maxGeometryVaryingComponents = 64;
        resources.maxTessControlInputComponents = 128;
        resources.maxTessControlOutputComponents = 128;
        resources.maxTessControlTextureImageUnits = 16;
        resources.maxTessControlUniformComponents = 1024;
        resources.maxTessControlTotalOutputComponents = 4096;
        resources.maxTessEvaluationInputComponents = 128;
        resources.maxTessEvaluationOutputComponents = 128;
        resources.maxTessEvaluationTextureImageUnits = 16;
        resources.maxTessEvaluationUniformComponents = 1024;
        resources.maxTessPatchComponents = 120;
        resources.maxPatchVertices = 32;
        resources.maxTessGenLevel = 64;
        resources.maxViewports = 16;
        resources.maxVertexAtomicCounters = 0;
        resources.maxTessControlAtomicCounters = 0;
        resources.maxTessEvaluationAtomicCounters = 0;
        resources.maxGeometryAtomicCounters = 0;
        resources.maxFragmentAtomicCounters = 8;
        resources.maxCombinedAtomicCounters = 8;
        resources.maxAtomicCounterBindings = 1;
        resources.maxVertexAtomicCounterBuffers = 0;
        resources.maxTessControlAtomicCounterBuffers = 0;
        resources.maxTessEvaluationAtomicCounterBuffers = 0;
        resources.maxGeometryAtomicCounterBuffers = 0;
        resources.maxFragmentAtomicCounterBuffers = 1;
        resources.maxCombinedAtomicCounterBuffers = 1;
        resources.maxAtomicCounterBufferSize = 16384;
        resources.maxTransformFeedbackBuffers = 4;
        resources.maxTransformFeedbackInterleavedComponents = 64;
        resources.maxCullDistances = 8;
        resources.maxCombinedClipAndCullDistances = 8;
        resources.maxSamples = 4;
        resources.limits.nonInductiveForLoops = true;
        resources.limits.whileLoops = true;
        resources.limits.doWhileLoops = true;
        resources.limits.generalUniformIndexing = true;
        resources.limits.generalAttributeMatrixVectorIndexing = true;
        resources.limits.generalVaryingIndexing = true;
        resources.limits.generalSamplerIndexing = true;
        resources.limits.generalVariableIndexing = true;
        resources.limits.generalConstantMatrixVectorIndexing = true;
        return resources;
    }


    VkShaderModule Shader::CreateShaderModule(const std::filesystem::path& file, const std::string &moduleCode, const std::string &preamble, VkShaderStageFlagBits stageFlags)
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        m_Stages.emplace_back(file);

        // Starts converting GLSL to SPIR-V
        auto language = GetEshLanguage(stageFlags);
        glslang::TProgram program;
        glslang::TShader shader(language);
        auto resources = GetResources();

        // Enable SPIR-V and Vulkan rules when parsing GLSL
        auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
        //messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);

        auto shaderName = file.string();
        auto shaderNameCstr = shaderName.c_str();
        auto shaderSource = moduleCode.c_str();
        shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderNameCstr, 1);
        shader.setPreamble(preamble.c_str());

        auto defaultVersion = glslang::EShTargetVulkan_1_0;
        shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
        shader.setEnvClient(glslang::EShClientVulkan, defaultVersion);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

        ShaderIncluder includer;
        std::string str;

        if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &str, includer))
        {
            _Magma_Core_Trace(shader.getInfoLog());
            _Magma_Core_Trace(shader.getInfoDebugLog());
            _Magma_Core_Error("SPIR-V shader preprocess failed");
        }

        if (!shader.parse(&resources, defaultVersion, true, messages, includer))
        {
            _Magma_Core_Trace(shader.getInfoLog());
            _Magma_Core_Trace(shader.getInfoDebugLog());
            _Magma_Core_Error("SPIR-V shader parse failed");
        }

        program.addShader(&shader);

        if (!program.link(messages) || !program.mapIO())
            _Magma_Core_Error("Error while linking shader program");

        program.buildReflection();

        for (uint32_t dim = 0; dim < 3; ++dim)
        {
            if (auto localSize = program.getLocalSize(dim); localSize > 1)
                m_LocalSizes[dim] = localSize;
        }

        for(int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
            LoadUniformBlock(program, stageFlags, i);

        for(int32_t i = program.getNumLiveUniformVariables() - 1; i >= 0; i--)
            LoadUniform(program, stageFlags, i);

        for(int32_t i = program.getNumLiveAttributes() - 1; i >= 0; i--)
            LoadAttribute(program, stageFlags, i);

        glslang::SpvOptions spvOptions;
        spvOptions.generateDebugInfo = false;
        spvOptions.disableOptimizer = false;
        spvOptions.optimizeSize = true;

        spv::SpvBuildLogger logger;
        std::vector<uint32_t> spirv;
        GlslangToSpv(*program.getIntermediate(static_cast<EShLanguage>(language)), spirv, &logger, &spvOptions);

        VkShaderModule module;
        VkShaderModuleCreateInfo createInfo{
          .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
          .codeSize = spirv.size() * sizeof(uint32_t),
          .pCode = reinterpret_cast<const uint32_t*>(spirv.data())
        };

        Graphics::CheckVk(vkCreateShaderModule(device, &createInfo, nullptr, &module));

        return module;
    }

    void Shader::LoadUniformBlock(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i)
    {
        const auto& reflection = program.getUniformBlock(i);

        for (auto &[uniformBlockName, uniformBlock] : m_UniformBlocks)
        {
            if (uniformBlockName == reflection.name)
            {
                uniformBlock.stageFlags |= stageFlag;
                return;
            }
        }

        auto type = UniformBlock::Type::None;
        if (reflection.getType()->getQualifier().storage == glslang::EvqUniform)
            type = UniformBlock::Type::Uniform;
        if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer)
            type = UniformBlock::Type::Storage;
        if (reflection.getType()->getQualifier().layoutPushConstant)
            type = UniformBlock::Type::Push;

        m_UniformBlocks.emplace(reflection.name, UniformBlock(reflection.getBinding(), reflection.size, stageFlag, type));
    }

    void Shader::LoadUniform(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i)
    {
        auto reflection = program.getUniform(i);

        if (reflection.getBinding() == -1)
        {
            auto splitName = String::Split(reflection.name, '.');

            if (splitName.size() > 1)
            {
                for (auto &[uniformBlockName, uniformBlock] : m_UniformBlocks)
                {
                    if (uniformBlockName == splitName.at(0))
                    {
                        uniformBlock.uniforms.emplace(String::ReplaceFirst(reflection.name, splitName.at(0) + ".", ""),
                                                      Uniform(reflection.getBinding(),
                                                              reflection.offset,
                                                              ComputeSize(reflection.getType()),
                                                              reflection.glDefineType,
                                                              false,
                                                              false,
                                                              stageFlag));
                        return;
                    }
                }
            }

            for (auto& [uniformName, uniform] : m_Uniforms)
            {
                if (uniformName == reflection.name)
                {
                    uniform.stageFlags |= stageFlag;
                    return;
                }
            }

            auto &qualifier = reflection.getType()->getQualifier();
            m_Uniforms.emplace(reflection.name, Uniform(reflection.getBinding(), reflection.offset, -1, reflection.glDefineType, qualifier.readonly, qualifier.writeonly, stageFlag));
        }
    }

    void Shader::LoadAttribute(glslang::TProgram& program, VkShaderStageFlagBits stageFlag, int32_t i)
    {
        auto reflection = program.getPipeInput(i);

        if (reflection.name.empty())
            return;

        for (const auto &[attributeName, attribute] : m_Attributes) {
            if (attributeName == reflection.name)
                return;
        }

        auto &qualifier = reflection.getType()->getQualifier();
        m_Attributes.emplace(reflection.name, Attribute(qualifier.layoutSet, qualifier.layoutLocation, ComputeSize(reflection.getType()), reflection.glDefineType));
    }

    int32_t Shader::ComputeSize(const glslang::TType *ttype)
    {
        int32_t components = 0;

        if (ttype->getBasicType() == glslang::EbtStruct || ttype->getBasicType() == glslang::EbtBlock) {
            for (const auto &tl : *ttype->getStruct())
                components += ComputeSize(tl.type);
        } else if (ttype->getMatrixCols() != 0) {
            components = ttype->getMatrixCols() * ttype->getMatrixRows();
        } else {
            components = ttype->getVectorSize();
        }

        if (ttype->getArraySizes()) {
            int32_t arraySize = 1;

            for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d) {
                // This only makes sense in paths that have a known array size.
                if (auto dimSize = ttype->getArraySizes()->getDimSize(d); dimSize != glslang::UnsizedArraySize)
                    arraySize *= dimSize;
            }

            components *= arraySize;
        }

        return sizeof(float) * components;
    }

    void CopyShaderStageCreateInfo(const std::vector<Shader>& src, std::vector<VkPipelineShaderStageCreateInfo>& dst)
    {
        /*dst.resize(src.size());
        for (int i = 0; i < src.size(); ++i)
            dst[i] = src[i].GetStageCreateInfo();*/
    }
}
