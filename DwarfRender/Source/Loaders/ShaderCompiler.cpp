#include <DwarfRender/Loaders/ShaderCompiler.h>

#include <DwarfResources/ResourceManager.h>

#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

#include "ShaderFileIncluder.h"

namespace {
	auto GetResources() -> TBuiltInResource {
		TBuiltInResource resources;
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
		resources.maxMeshOutputVerticesNV = 256;
		resources.maxMeshOutputPrimitivesNV = 512;
		resources.maxMeshWorkGroupSizeX_NV = 32;
		resources.maxMeshWorkGroupSizeY_NV = 1;
		resources.maxMeshWorkGroupSizeZ_NV = 1;
		resources.maxTaskWorkGroupSizeX_NV = 32;
		resources.maxTaskWorkGroupSizeY_NV = 1;
		resources.maxTaskWorkGroupSizeZ_NV = 1;
		resources.maxMeshViewCountNV = 4;

		resources.limits.nonInductiveForLoops = 1;
		resources.limits.whileLoops = 1;
		resources.limits.doWhileLoops = 1;
		resources.limits.generalUniformIndexing = 1;
		resources.limits.generalAttributeMatrixVectorIndexing = 1;
		resources.limits.generalVaryingIndexing = 1;
		resources.limits.generalSamplerIndexing = 1;
		resources.limits.generalVariableIndexing = 1;
		resources.limits.generalConstantMatrixVectorIndexing = 1;

		return resources;
	}

	const char* Preamble =
		"#version 450\n"
		"#extension GL_ARB_separate_shader_objects : enable\n"
		"#extension GL_GOOGLE_include_directive : enable\n";
}

rf::ShaderCompiler::ShaderCompiler() {
	glslang::InitializeProcess();
}

rf::ShaderCompiler::~ShaderCompiler() {
	glslang::FinalizeProcess();
}

auto rf::ShaderCompiler::CompileShader(
	const df::String& shaderName, 
	const char* code, 
	EShaderType shaderType, 
	const df::ResourceManager& resourceManager,
	const rf::ParamSetDefinitionManager& paramSetDefinitionManager)->ShaderCompileInfo {

	ShaderCompileInfo info;

	ClearLog();

	EShLanguage shaderLanguage = EShLanguage::EShLangCount;
	switch (shaderType) {
	case EShaderType::Vertex:
		shaderLanguage = EShLanguage::EShLangVertex;
		break;
	case EShaderType::Geometry:
		shaderLanguage = EShLanguage::EShLangGeometry;
		break;
	case EShaderType::Fragment:
		shaderLanguage = EShLanguage::EShLangFragment;
		break;
	case EShaderType::Compute:
		shaderLanguage = EShLanguage::EShLangCompute;
		break;
	case EShaderType::Mesh:
		shaderLanguage = EShLanguage::EShLangMeshNV;
		break;
	default:
		break;
	}

	const char* inputData[2] = { Preamble, code };
	const int defaultVersion = 100;
	const int version = 100;
	const EShMessages messages = EShMessages(EShMsgSpvRules | EShMsgVulkanRules);
	const glslang::EShTargetClientVersion vulkanClientVersion = glslang::EShTargetVulkan_1_0;
	const glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;

	TBuiltInResource resources = GetResources();

	glslang::TShader shader = glslang::TShader(shaderLanguage);
	shader.setStrings(inputData, 2);
	shader.setEnvInput(glslang::EShSourceGlsl, shaderLanguage, glslang::EShClientVulkan, version);
	shader.setEnvClient(glslang::EShClientVulkan, vulkanClientVersion);
	shader.setEnvTarget(glslang::EShTargetSpv, targetVersion);

	ShaderFileIncluder includer = ShaderFileIncluder(resourceManager, paramSetDefinitionManager, info);

	df::String preprocessedCode;
	if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &preprocessedCode, includer)) {
		AddLogLine("GLSL Preprocessing Failed for: ", shaderName);
		AddLogLine(shader.getInfoLog());
		AddLogLine(shader.getInfoDebugLog());

		return info;
	}

	const char* preprocessedInputData = preprocessedCode.c_str();

	shader.setStrings(&preprocessedInputData, 1);

	if (!shader.parse(&resources, defaultVersion, false, messages)) {
		AddLogLine("GLSL Parsing Failed for: ", shaderName);
		AddLogLine(shader.getInfoLog());
		AddLogLine(shader.getInfoDebugLog());

		return info;
	}

	glslang::TProgram program;
	program.addShader(&shader);

	if (!program.link(messages)) {
		AddLogLine("GLSL Linking Failed for: ", shaderName);
		AddLogLine(shader.getInfoLog());
		AddLogLine(shader.getInfoDebugLog());

		return info;
	}

	spv::SpvBuildLogger logger;
	glslang::SpvOptions spvOptions;

#if 0
	spvOptions.generateDebugInfo = false;
	spvOptions.disableOptimizer = false;
	spvOptions.optimizeSize = true;
	spvOptions.disassemble = false;
	spvOptions.validate = false;
#endif

	glslang::GlslangToSpv(*program.getIntermediate(shaderLanguage), info.m_SpirVCode, &logger, &spvOptions);

	if (logger.getAllMessages().length() != 0) {
		AddLogLine(logger.getAllMessages());
	}

	return info;
}

auto rf::ShaderCompiler::GetLog() const ->const df::String& {
	return m_Log;
}
void rf::ShaderCompiler::ClearLog() {
	m_Log.clear();
}

void rf::ShaderCompiler::AddLogLine(const df::StringView& line) {
	m_Log += line;
	m_Log += "\n";
}

void rf::ShaderCompiler::AddLogLine(const df::StringView& line, const df::StringView& arg) {
	m_Log += line;
	m_Log += arg;
	m_Log += "\n";
}