#include "VkPipeline.h"
#include "VkRenderCore.h"
#include "VkDebug.h"
#include "VkParameterSetDefinition.h"
#include "VkParameterSet.h"
#include "VkShaderCompiler.h"
#include "VkShaderParsing.h"

#include <DwarvenCore/StringOperations.h>

#include <iostream>

bool vk::PipelineState::operator == (const PipelineState& other) const {
	return
		m_BlendState == other.m_BlendState &&
		m_DepthState == other.m_DepthState &&
		m_RasterizationState == other.m_RasterizationState &&
		m_PrimitiveTopology == other.m_PrimitiveTopology &&
		m_RenderPass == other.m_RenderPass;
}

bool vk::PipelineState::operator != (const PipelineState& other) const {
	return !(operator == (other));
}


vk::Pipeline::PipelineStateObjectSlot::PipelineStateObjectSlot(const vk::PipelineState& state, const VkPipeline vkPipeline) 
	: m_State(state)
	, m_PipelineStateObject(vkPipeline) {
}

vk::Pipeline::Pipeline(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore) 
	, m_VkVertexShaderModule(VK_NULL_HANDLE)
	, m_VkFragmentShaderModule(VK_NULL_HANDLE) 
	, m_VkPipelineLayout(VK_NULL_HANDLE)
	, m_IsBuilt(false) {
}

vk::Pipeline::~Pipeline() {
	if (m_VkPipelineLayout != VK_NULL_HANDLE) {
		m_RenderCore.RemovePipelineLayout(m_VkPipelineLayout);
	}

	if (m_VkVertexShaderModule != VK_NULL_HANDLE) {
		m_RenderCore.RemoveShaderModule(m_VkVertexShaderModule);
	}

	if (m_VkFragmentShaderModule != VK_NULL_HANDLE) {
		m_RenderCore.RemoveShaderModule(m_VkFragmentShaderModule);
	}
}

void vk::Pipeline::DeclareName(const df::StringView& name) {
	DFAssert(!m_IsBuilt, "Casn't set pipeline name - pipeline is already built!");

	m_Name = name;
}

void vk::Pipeline::DeclareVertexShader(const df::StringView& code) {
	DFAssert(!m_IsBuilt, "Casn't set pipeline shader - pipeline is already built!");

	m_VertexShaderCode = df::String(code);
}

void vk::Pipeline::DeclareFragmentShader(const df::StringView& code) {
	DFAssert(!m_IsBuilt, "Casn't set pipeline shader - pipeline is already built!");

	m_FragmentShaderCode = df::String(code);
}

bool vk::Pipeline::Build() {
	auto shaderCompiler = m_RenderCore.GetShaderCompiler();

	const df::String vsCode = ParseShader(m_VertexShaderCode);
	const df::String fsCode = ParseShader(m_FragmentShaderCode);

	// Vertex Shader
	if (!vsCode.empty()) {

		df::Vector<df::String> generated;
		df::Vector<const char*> codeLines;

		for (uint32 i = 0; i < uint32(m_ParameterSetDefinitions.size()); ++i) {
			const auto& def = m_ParameterSetDefinitions[i];
			generated.emplace_back(def->MakeShaderSnippet(i));
		}

		for (uint32 i = 0; i < uint32(m_VertexAttributes.size()); ++i) {
			const auto& attr = m_VertexAttributes[i];
			generated.push_back(attr->m_ShaderString);
		}

		for (auto& str : generated) {
			codeLines.push_back(str.c_str());
		}
		codeLines.push_back(vsCode.c_str());


		const auto spirVCode = shaderCompiler->CompileShader(m_Name, codeLines, vk::EShaderType::Vertex);

		const auto& log = shaderCompiler->GetLog();
		if (!log.empty()) {
			std::cout << "Vertex Shader Compile Error:\n" << log << "\n";
			shaderCompiler->ClearLog();

			return false;
		} else {
			m_VkVertexShaderModule = CreateShaderModule(spirVCode.data(), uint32(spirVCode.size()));
		}
	}

	// Fragment Shader
	if (!fsCode.empty()) {

		df::Vector<df::String> generated;
		df::Vector<const char*> codeLines;

		for (uint32 i = 0; i < uint32(m_ParameterSetDefinitions.size()); ++i) {
			const auto& def = m_ParameterSetDefinitions[i];
			generated.emplace_back(def->MakeShaderSnippet(i));
		}

		for (auto& str : generated) {
			codeLines.push_back(str.c_str());
		}
		codeLines.push_back(fsCode.c_str());

		const auto spirVCode = shaderCompiler->CompileShader(m_Name, codeLines, vk::EShaderType::Fragment);

		const auto& log = shaderCompiler->GetLog();
		if (!log.empty()) {
			std::cout << "Fragment Shader Compile Error:\n" << log << "\n";
			shaderCompiler->ClearLog();

			return false;
		} else {
			m_VkFragmentShaderModule = CreateShaderModule(spirVCode.data(), uint32(spirVCode.size()));
		}
	}

	// Pipeline Layout
	m_VkPipelineLayout = CreateLayout();

	// Vertex Description
	CreateVertexDescription();

	m_IsBuilt = true;

	return true;
}

void vk::Pipeline::SetBlendEnabled(bool value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_BlendEnable = value;
}

void vk::Pipeline::SetColorBlendOp(df::EBlendOp value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_ColorBlendOp = value;
}

void vk::Pipeline::SetAlphaBlendOp(df::EBlendOp value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_AlphaBlendOp = value;
}

void vk::Pipeline::SetSrcColorBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_SrcColorBlendFactor = value;
}

void vk::Pipeline::SetSrcAlphaBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_SrcAlphaBlendFactor = value;
}

void vk::Pipeline::SetDstColorBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_DstColorBlendFactor = value;
}

auto vk::Pipeline::CreatePipelineStateObject()->VkPipeline {
	DFAssert(m_IsBuilt, "Can't create pipeline state object - pipeline is not built yet!");
	DFAssert(m_State.m_RenderPass != nullptr, "There is no active render pass to build pipeline state object!");




	VkPipeline vkPipeline = VK_NULL_HANDLE;



	m_PipelineStateObjects.push_back(PipelineStateObjectSlot(m_State, vkPipeline));
	return vkPipeline;
}

auto vk::Pipeline::ParseShader(const df::StringView& code)->df::String {
	using namespace vk_shader_parsing;

	df::String outCode;
	outCode.reserve(1024);

	for (auto&& line : df::Tokenize(code, "\n\r")) {
		if (line.empty()) {
			continue;
		}

		bool parsed = false;

		if (const auto vertexAttribute =  FetchVertexAttribute(line, m_RenderCore)) {
			df::AddUnique(m_VertexAttributes, vertexAttribute.Get());

			parsed = true;
		} else if (const auto parameterSet = FetchParameterSet(line, m_RenderCore)) {
			df::AddUnique(m_ParameterSetDefinitions, parameterSet.Get());

			parsed = true;
		} else if (const auto depthTest = FetchDepthTest(line)) {
			m_State.m_DepthState.m_DepthTestEnable = depthTest.Get();

			parsed = true;
		} else if (const auto depthWrite = FetchDepthWrite(line)) {
			m_State.m_DepthState.m_DepthWriteEnable = depthWrite.Get();

			parsed = true;
		} else if (const auto stencilTest = FetchStencilTest(line)) {
			m_State.m_DepthState.m_StencilTestEnable = stencilTest.Get();

			parsed = true;
		} else if (const auto depthCompareOp = FetchDepthCompareOp(line)) {
			m_State.m_DepthState.m_DepthCompareOp = depthCompareOp.Get();

			parsed = true;
		} else if (const auto stencilOp = FetchStencilOp(line)) {
			m_State.m_DepthState.m_StencilOp = stencilOp.Get();

			parsed = true;
		} else if (const auto polygonMode = FetchPolygonMode(line)) {
			m_State.m_RasterizationState.m_PolygonMode = polygonMode.Get();

			parsed = true;
		} else if (const auto frontFace = FetchFrontFace(line)) {
			m_State.m_RasterizationState.m_FrontFace = frontFace.Get();

			parsed = true;
		} else if (const auto cullMode = FetchCullMode(line)) {
			m_State.m_RasterizationState.m_CullMode = cullMode.Get();

			parsed = true;
		} else if (const auto blendEnable = FetchBlendEnable(line)) {
			m_State.m_BlendState.m_BlendEnable = blendEnable.Get();

			parsed = true;
		} else if (const auto colorBlendOp = FetchColorBlendOp(line)) {
			m_State.m_BlendState.m_ColorBlendOp = colorBlendOp.Get();

			parsed = true;
		} else if (const auto alphaBlendOp = FetchAlphaBlendOp(line)) {
			m_State.m_BlendState.m_AlphaBlendOp = alphaBlendOp.Get();

			parsed = true;
		} else if (const auto srcColorBlendFactor = FetchSrcColorBlendFactor(line)) {
			m_State.m_BlendState.m_SrcColorBlendFactor = srcColorBlendFactor.Get();

			parsed = true;
		} else if (const auto dstColorBlendFactor = FetchDstColorBlendFactor(line)) {
			m_State.m_BlendState.m_DstColorBlendFactor = dstColorBlendFactor.Get();

			parsed = true;
		} else if (const auto srcAlphaBlendFactor = FetchSrcAlphaBlendFactor(line)) {
			m_State.m_BlendState.m_SrcAlphaBlendFactor = srcAlphaBlendFactor.Get();

			parsed = true;
		} else if (const auto dstAlphaBlendFactor = FetchDstAlphaBlendFactor(line)) {
			m_State.m_BlendState.m_DstAlphaBlendFactor = dstAlphaBlendFactor.Get();

			parsed = true;
		} else if (const auto blendState = FetchBlendState(line)) {
			m_State.m_BlendState = blendState.Get();

			parsed = true;
		}

		if (!parsed) {
			outCode += line;
		}
		outCode += "\n";
	}

	return outCode;
}

void vk::Pipeline::SetDstAlphaBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_DstAlphaBlendFactor = value;
}

void vk::Pipeline::SetBlendState(df::EBlendState value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState = value;
}

void vk::Pipeline::SetDepthTestEnabled(bool value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthTestEnable = value;
}

void vk::Pipeline::SetDepthWriteEnabled(bool value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthWriteEnable = value;
}

void vk::Pipeline::SetStencilTestEnabled(bool value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_StencilTestEnable = value;
}

void vk::Pipeline::SetDepthCompareOp(df::EDepthCompareOp value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthCompareOp = value;
}

void vk::Pipeline::SetStencilOp(df::EStencilOp value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_StencilOp = value;
}

void vk::Pipeline::SetDepthStencilState(df::EDepthStencilState value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState = value;
}

void vk::Pipeline::SetRasterizerDiscardEnabled(bool value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_RasterizerDiscardEnable = value;
}

void vk::Pipeline::SetPolygonMode(df::EPolygonMode value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_PolygonMode = value;
}

void vk::Pipeline::SetFrontFace(df::EFrontFace value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_FrontFace = value;
}

void vk::Pipeline::SetCullMode(df::ECullMode value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_CullMode = value;
}

void vk::Pipeline::SetRasterizationState(df::ERasterizationState value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState = value;
}

void vk::Pipeline::SetPrimitiveTopology(df::EPrimitiveTopology value) {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	m_State.m_PrimitiveTopology = value;
}

auto vk::Pipeline::GetParameterSetSlot(vk::ParameterSet* parameterSet) const -> int32 {
	DFAssert(m_IsBuilt, "Casn't set pipeline property - pipeline is not built yet!");

	const auto* definition = &parameterSet->GetDefinition();

	for(size_t i=0; i< m_ParameterSetDefinitions.size(); ++i) {
		auto&& def = m_ParameterSetDefinitions[i];
		if (def == definition) {
			return static_cast<int32>(i);
		}
	}

	return -1;
}

auto vk::Pipeline::GetPipelineForState(const vk::RenderPass* renderPass)->VkPipeline {
	m_State.m_RenderPass = renderPass;

	for (const auto& psoState : m_PipelineStateObjects) {
		if (psoState.m_State == m_State) {
			return psoState.m_PipelineStateObject;
		}
	}

	return CreatePipelineStateObject();
}

auto vk::Pipeline::GetVkPipelineLayout() const->VkPipelineLayout {
	return m_VkPipelineLayout;
}

auto vk::Pipeline::CreateShaderModule(const uint32* data, uint32 length) -> VkShaderModule {
	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = length * sizeof(uint32);
	shaderModuleCreateInfo.pCode = data;

	VkShaderModule vkShaderModule = VK_NULL_HANDLE;
	if (vk::API::CreateShaderModule(vkDevice, &shaderModuleCreateInfo, vk::Allocator(), &vkShaderModule) != VK_SUCCESS) {
		DFAssert(false, "Can't create ShaderModule!");
	}

	DFVkDebugName(vkDevice, vkShaderModule, m_Name);

	return vkShaderModule;
}

auto vk::Pipeline::CreateLayout()->VkPipelineLayout {
	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	const uint32 descriptorSetLayoutCount = uint32(m_ParameterSetDefinitions.size());
	df::Vector<VkDescriptorSetLayout> descriptorSetLayouts(descriptorSetLayoutCount);
	for (uint32 i = 0; i < descriptorSetLayoutCount; ++i) {
		descriptorSetLayouts[i] = m_ParameterSetDefinitions[i]->GetVkDescriptorSetLayout();
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayoutCount;
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
	if (vk::API::CreatePipelineLayout(vkDevice, &pipelineLayoutInfo, vk::Allocator(), &vkPipelineLayout) != VK_SUCCESS) {
		DFAssert(false, "Can't create PipelineLayout!");
	}

	DFVkDebugName(vkDevice, vkPipelineLayout, m_Name);

	return vkPipelineLayout;
}

void vk::Pipeline::CreateVertexDescription() {
	m_BindingDescriptions.resize(m_VertexAttributes.size());
	m_AttributeDescriptions.resize(m_VertexAttributes.size());

	for (size_t i = 0; i < m_VertexAttributes.size(); ++i) {
		m_BindingDescriptions[i] = {};
		m_BindingDescriptions[i].binding = m_VertexAttributes[i]->m_Index;
		m_BindingDescriptions[i].stride = df::ToStride(m_VertexAttributes[i]->m_Format);
		m_BindingDescriptions[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		m_AttributeDescriptions[i] = {};
		m_AttributeDescriptions[i].binding = uint32(i);
		m_AttributeDescriptions[i].location = uint32(i);
		m_AttributeDescriptions[i].format = df::ToVkFormat(m_VertexAttributes[i]->m_Format);
		m_AttributeDescriptions[i].offset = 0;
	}
}