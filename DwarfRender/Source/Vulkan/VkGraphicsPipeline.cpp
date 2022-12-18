#include "VkGraphicsPipeline.h"
#include "VkRenderCore.h"
#include "VkDebug.h"
#include "VkParameterSetDefinition.h"
#include "VkParameterSet.h"
#include "VkRenderPass.h"

#include <DwarvenCore/StringOperations.h>

#include <iostream>

namespace {
	auto ToVkPolygonMode(df::EPolygonMode value)->VkPolygonMode {
		switch (value) {
		case df::EPolygonMode::Fill:
			return VK_POLYGON_MODE_FILL;
		case df::EPolygonMode::Line:
			return VK_POLYGON_MODE_LINE;
		case df::EPolygonMode::Point:
			return VK_POLYGON_MODE_POINT;
		default:
			return VK_POLYGON_MODE_FILL;
		}
	}

	auto ToVkCullMode(df::ECullMode value)->VkCullModeFlagBits {
		switch (value) {
		case df::ECullMode::None:
			return VK_CULL_MODE_NONE;
		case df::ECullMode::Front:
			return VK_CULL_MODE_FRONT_BIT;
		case df::ECullMode::Back:
			return VK_CULL_MODE_BACK_BIT;
		case df::ECullMode::FrontAndBack:
			return VK_CULL_MODE_FRONT_AND_BACK;
		default:
			return VK_CULL_MODE_NONE;
		}
	}

	auto ToVkFrontFace(df::EFrontFace value)->VkFrontFace {
		switch (value) {
		case df::EFrontFace::CounterClockwise:
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		case df::EFrontFace::Clockwise:
			return VK_FRONT_FACE_CLOCKWISE;
		default:
			return VK_FRONT_FACE_CLOCKWISE;
		}
	}

	auto ToVkBlendFactor(df::EBlendFactor value)->VkBlendFactor {
		switch (value) {
		case df::EBlendFactor::Zero:
			return VK_BLEND_FACTOR_ZERO;
		case df::EBlendFactor::One:
			return VK_BLEND_FACTOR_ONE;
		case df::EBlendFactor::SrcColor:
			return VK_BLEND_FACTOR_SRC_COLOR;
		case df::EBlendFactor::OneMinusSrcColor:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case df::EBlendFactor::DstColor:
			return VK_BLEND_FACTOR_DST_COLOR;
		case df::EBlendFactor::OneMinusDstColor:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case df::EBlendFactor::SrcAlpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;
		case df::EBlendFactor::OneMinusSrcAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case df::EBlendFactor::DstAlpha:
			return VK_BLEND_FACTOR_DST_ALPHA;
		case df::EBlendFactor::OneMinusDstAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		default:
			return VK_BLEND_FACTOR_ZERO;
		}
	}

	auto ToVkBlendOp(df::EBlendOp value)->VkBlendOp {
		switch (value) {
		case df::EBlendOp::Add:
			return VK_BLEND_OP_ADD;
		case df::EBlendOp::Subtract:
			return VK_BLEND_OP_SUBTRACT;
		case df::EBlendOp::ReverseSubtract:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
		case df::EBlendOp::Min:
			return VK_BLEND_OP_MIN;
		case df::EBlendOp::Max:
			return VK_BLEND_OP_MAX;
		default:
			return VK_BLEND_OP_ADD;
		}
	}

	auto ToVkDepthCompareOp(df::EDepthCompareOp value)->VkCompareOp {
		switch (value) {
		case df::EDepthCompareOp::Never:
			return VK_COMPARE_OP_NEVER;
		case df::EDepthCompareOp::Equal:
			return VK_COMPARE_OP_EQUAL;
		case df::EDepthCompareOp::NotEqual:
			return VK_COMPARE_OP_NOT_EQUAL;
		case df::EDepthCompareOp::Greater:
			return VK_COMPARE_OP_GREATER;
		case df::EDepthCompareOp::GreaterEqual:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case df::EDepthCompareOp::Less:
			return VK_COMPARE_OP_LESS;
		case df::EDepthCompareOp::LessEqual:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		case df::EDepthCompareOp::Always:
			return VK_COMPARE_OP_ALWAYS;
		default:
			return VK_COMPARE_OP_EQUAL;
		}
	}

	auto ToVkStencilOp(df::EStencilOp value)->VkStencilOp {
		switch (value) {
		case df::EStencilOp::Keep:
			return VK_STENCIL_OP_KEEP;
		case df::EStencilOp::Zero:
			return VK_STENCIL_OP_ZERO;
		case df::EStencilOp::Replace:
			return VK_STENCIL_OP_REPLACE;
		case df::EStencilOp::Increment:
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		case df::EStencilOp::Decrement:
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		case df::EStencilOp::Invert:
			return VK_STENCIL_OP_INVERT;
		default:
			return VK_STENCIL_OP_KEEP;
		}
	}

	auto ToVkPrimitiveTopology(df::EPrimitiveTopology topology)->VkPrimitiveTopology {
		switch (topology) {
		case df::EPrimitiveTopology::Points:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case df::EPrimitiveTopology::Lines:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case df::EPrimitiveTopology::Triangles:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		default:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	auto ToVkPipelineRasterizationStateCreateInfo(const df::RasterizationState& rasterizationState)->VkPipelineRasterizationStateCreateInfo {
		VkPipelineRasterizationStateCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthClampEnable = VK_FALSE;
		createInfo.rasterizerDiscardEnable = rasterizationState.m_RasterizerDiscardEnable;
		createInfo.polygonMode = ToVkPolygonMode(rasterizationState.m_PolygonMode);
		createInfo.cullMode = ToVkCullMode(rasterizationState.m_CullMode);
		createInfo.frontFace = ToVkFrontFace(rasterizationState.m_FrontFace);
		createInfo.depthBiasEnable = VK_FALSE;
		createInfo.depthBiasConstantFactor = 0.0f;
		createInfo.depthBiasClamp = 0.0f;
		createInfo.depthBiasSlopeFactor = 0.0f;
		createInfo.lineWidth = 1.0f;

		return createInfo;
	}

	void ToVkPipelineColorBlendStateCreateInfo(VkPipelineColorBlendStateCreateInfo& createInfo, df::Vector<VkPipelineColorBlendAttachmentState>& states, const df::BlendState& blendState, uint32 count) {

		VkPipelineColorBlendAttachmentState attachmentState = {};
		attachmentState.blendEnable = blendState.m_BlendEnable;

		attachmentState.srcColorBlendFactor = ToVkBlendFactor(blendState.m_SrcColorBlendFactor);
		attachmentState.dstColorBlendFactor = ToVkBlendFactor(blendState.m_DstColorBlendFactor);
		attachmentState.colorBlendOp = ToVkBlendOp(blendState.m_ColorBlendOp);
		attachmentState.srcAlphaBlendFactor = ToVkBlendFactor(blendState.m_SrcAlphaBlendFactor);
		attachmentState.dstAlphaBlendFactor = ToVkBlendFactor(blendState.m_DstAlphaBlendFactor);
		attachmentState.alphaBlendOp = ToVkBlendOp(blendState.m_AlphaBlendOp);
		attachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		states = {};
		states.resize(count);

		for (auto& it : states) {
			it = attachmentState;
		}

		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.logicOpEnable = VK_FALSE;
		createInfo.logicOp = VK_LOGIC_OP_COPY;
		createInfo.attachmentCount = count;
		createInfo.pAttachments = states.data();
		createInfo.blendConstants[0] = 0.0f;
		createInfo.blendConstants[1] = 0.0f;
		createInfo.blendConstants[2] = 0.0f;
		createInfo.blendConstants[3] = 0.0f;
	}

	auto ToVkPipelineDepthStencilStateCreateInfo(const df::DepthState& depthState) ->VkPipelineDepthStencilStateCreateInfo {
		VkPipelineDepthStencilStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthTestEnable = depthState.m_DepthTestEnable;
		createInfo.depthWriteEnable = depthState.m_DepthWriteEnable;
		createInfo.depthCompareOp = ToVkDepthCompareOp(depthState.m_DepthCompareOp);
		createInfo.depthBoundsTestEnable = VK_FALSE;
		createInfo.stencilTestEnable = depthState.m_StencilTestEnable;
		createInfo.front = {};
		createInfo.back = {};
		createInfo.minDepthBounds = 0.0f;
		createInfo.maxDepthBounds = 1.0f;

		return createInfo;
	}
}

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


vk::GraphicsPipeline::PipelineStateObjectSlot::PipelineStateObjectSlot(const vk::PipelineState& state, const VkPipeline vkPipeline) 
	: m_State(state)
	, m_PipelineStateObject(vkPipeline) {
}

vk::GraphicsPipeline::GraphicsPipeline(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore)
	, m_VkVertexShaderModule(VK_NULL_HANDLE)
	, m_VkFragmentShaderModule(VK_NULL_HANDLE) 
	, m_VkPipelineLayout(VK_NULL_HANDLE)
	, m_IsBuilt(false) {
}

vk::GraphicsPipeline::~GraphicsPipeline() {
	for (auto&& state : m_PipelineStateObjects) {
		m_RenderCore.RemovePipeline(state.m_PipelineStateObject);
	}

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

void vk::GraphicsPipeline::SetName(const df::StringView& name) {
	DFAssert(!m_IsBuilt, "Casn't set pipeline name - pipeline is already built!");

	m_Name = name;

	UpdateDebugNames();
}

void vk::GraphicsPipeline::SetupVertexShader(const df::Vector<uint32> &bytecode) {
	DFAssert(!m_IsBuilt, "Can't set up pipeline vertex shader - pipeline is already built!");

	m_VertexShaderSpirV = bytecode;
}

void vk::GraphicsPipeline::SetupFragmentShader(const df::Vector<uint32> &bytecode) {
	DFAssert(!m_IsBuilt, "Can't set up pipeline fragment shader - pipeline is already built!");

	m_FragmentShaderSpirV = bytecode;
}

void vk::GraphicsPipeline::SetupVertexAttributes(const df::Vector<df::String> &vertexAttributes) {
	m_VertexAttributes.resize(vertexAttributes.size());

	for (size_t i = 0; i < m_VertexAttributes.size(); ++i) {
		m_VertexAttributes[i] = m_RenderCore.FindVertexAttribute(vertexAttributes[i]);
		m_RequiredVertexAttributeBits[m_VertexAttributes[i]->m_Index] = true;
	}
}

void vk::GraphicsPipeline::SetupParameterSets(const df::Vector<df::String> &parameterSets) {
	m_ParameterSetDefinitions.resize(parameterSets.size());

	for(size_t i = 0; i < m_ParameterSetDefinitions.size(); ++i) {
		m_ParameterSetDefinitions[i] = m_RenderCore.FindParameterSetDefinition(parameterSets[i]);
	}
}

bool vk::GraphicsPipeline::Build() {
	// Vertex Shader
	if (!m_VertexShaderSpirV.empty()) {
		m_VkVertexShaderModule = CreateShaderModule(m_VertexShaderSpirV.data(), uint32(m_VertexShaderSpirV.size()));
	}

	// Fragment Shader
	if (!m_FragmentShaderSpirV.empty()) {
		m_VkFragmentShaderModule = CreateShaderModule(m_FragmentShaderSpirV.data(), uint32(m_FragmentShaderSpirV.size()));
	}

	// Pipeline Layout
	m_VkPipelineLayout = CreateLayout();

	// Vertex Description
	CreateVertexDescription();

	m_IsBuilt = true;

	return true;
}

void vk::GraphicsPipeline::SetBlendEnabled(bool value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_BlendEnable = value;
}

void vk::GraphicsPipeline::SetColorBlendOp(df::EBlendOp value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_ColorBlendOp = value;
}

void vk::GraphicsPipeline::SetAlphaBlendOp(df::EBlendOp value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_AlphaBlendOp = value;
}

void vk::GraphicsPipeline::SetSrcColorBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_SrcColorBlendFactor = value;
}

void vk::GraphicsPipeline::SetSrcAlphaBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_SrcAlphaBlendFactor = value;
}

void vk::GraphicsPipeline::SetDstColorBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_DstColorBlendFactor = value;
}

void vk::GraphicsPipeline::SetDstAlphaBlendFactor(df::EBlendFactor value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState.m_DstAlphaBlendFactor = value;
}

void vk::GraphicsPipeline::SetBlendState(df::EBlendState value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_BlendState = value;
}

void vk::GraphicsPipeline::SetDepthTestEnabled(bool value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthTestEnable = value;
}

void vk::GraphicsPipeline::SetDepthWriteEnabled(bool value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthWriteEnable = value;
}

void vk::GraphicsPipeline::SetStencilTestEnabled(bool value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_StencilTestEnable = value;
}

void vk::GraphicsPipeline::SetDepthCompareOp(df::EDepthCompareOp value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_DepthCompareOp = value;
}

void vk::GraphicsPipeline::SetStencilOp(df::EStencilOp value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState.m_StencilOp = value;
}

void vk::GraphicsPipeline::SetDepthStencilState(df::EDepthStencilState value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_DepthState = value;
}

void vk::GraphicsPipeline::SetRasterizerDiscardEnabled(bool value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_RasterizerDiscardEnable = value;
}

void vk::GraphicsPipeline::SetPolygonMode(df::EPolygonMode value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_PolygonMode = value;
}

void vk::GraphicsPipeline::SetFrontFace(df::EFrontFace value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_FrontFace = value;
}

void vk::GraphicsPipeline::SetCullMode(df::ECullMode value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState.m_CullMode = value;
}

void vk::GraphicsPipeline::SetRasterizationState(df::ERasterizationState value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_RasterizationState = value;
}

void vk::GraphicsPipeline::SetPrimitiveTopology(df::EPrimitiveTopology value) {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	m_State.m_PrimitiveTopology = value;
}

auto vk::GraphicsPipeline::GetVertexAttributeBits() const -> const df::Bitset<64>& {
	return m_RequiredVertexAttributeBits;
}

auto vk::GraphicsPipeline::GetParameterSetSlot(vk::ParameterSet* parameterSet) const -> int32 {
	DFAssert(m_IsBuilt, "Can't set pipeline property - pipeline is not built yet!");

	const auto* definition = &parameterSet->GetDefinition();

	for(size_t i=0; i< m_ParameterSetDefinitions.size(); ++i) {
		auto&& def = m_ParameterSetDefinitions[i];
		if (def == definition) {
			return static_cast<int32>(i);
		}
	}

	return -1;
}

auto vk::GraphicsPipeline::GetPipelineForState(const vk::RenderPass* renderPass)->VkPipeline {
	m_State.m_RenderPass = renderPass;

	for (const auto& psoState : m_PipelineStateObjects) {
		if (psoState.m_State == m_State) {
			return psoState.m_PipelineStateObject;
		}
	}

	return CreatePipelineStateObject();
}

auto vk::GraphicsPipeline::GetVkPipelineLayout() const->VkPipelineLayout {
	return m_VkPipelineLayout;
}

auto vk::GraphicsPipeline::CreateShaderModule(const uint32* data, uint32 length) -> VkShaderModule {
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

auto vk::GraphicsPipeline::CreateLayout()->VkPipelineLayout {
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

void vk::GraphicsPipeline::CreateVertexDescription() {
	m_BindingDescriptions.resize(m_VertexAttributes.size());
	m_AttributeDescriptions.resize(m_VertexAttributes.size());

	for (size_t i = 0; i < m_VertexAttributes.size(); ++i) {
		m_BindingDescriptions[i] = {};
		m_BindingDescriptions[i].binding = m_VertexAttributes[i]->m_Index;
		m_BindingDescriptions[i].stride = ToDataStride(m_VertexAttributes[i]->m_Format);
		m_BindingDescriptions[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		m_AttributeDescriptions[i] = {};
		m_AttributeDescriptions[i].binding = m_VertexAttributes[i]->m_Index;
		m_AttributeDescriptions[i].location = uint32(i);
		m_AttributeDescriptions[i].format = ToVkFormat(m_VertexAttributes[i]->m_Format);
		m_AttributeDescriptions[i].offset = 0;
	}
}

void vk::GraphicsPipeline::UpdateDebugNames() {
	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	if (m_VkVertexShaderModule != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkVertexShaderModule, m_Name);
	}

	if (m_VkFragmentShaderModule != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkFragmentShaderModule, m_Name);
	}

	if (m_VkPipelineLayout != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkPipelineLayout, m_Name);
	}

	for (auto&& pso : m_PipelineStateObjects) {
		DFVkDebugName(vkDevice, pso.m_PipelineStateObject, m_Name);
	}
}


auto vk::GraphicsPipeline::CreatePipelineStateObject()->VkPipeline {
	DFAssert(m_IsBuilt, "Can't create pipeline state object - pipeline is not built yet!");
	DFAssert(m_State.m_RenderPass != nullptr, "There is no active render pass to build pipeline state object!");

	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	df::Vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
	if (m_VkVertexShaderModule != VK_NULL_HANDLE) {
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		pipelineShaderStageCreateInfo.module = m_VkVertexShaderModule;
		pipelineShaderStageCreateInfo.pName = "main";

		shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
	}
	if (m_VkFragmentShaderModule != VK_NULL_HANDLE) {
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		pipelineShaderStageCreateInfo.module = m_VkFragmentShaderModule;
		pipelineShaderStageCreateInfo.pName = "main";

		shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
	}

	df::Vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	const uint32 colorAttachmentCount = m_State.m_RenderPass->GetColorAttachmentCount();

	VkPipelineColorBlendStateCreateInfo blendStateCreateInfo;
	df::Vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates;
	ToVkPipelineColorBlendStateCreateInfo(blendStateCreateInfo, blendAttachmentStates, m_State.m_BlendState, colorAttachmentCount);

	const auto& vertexInputBindingDescriptions = m_BindingDescriptions;
	const auto& vertexInputAttributeDescriptions = m_AttributeDescriptions;

	const VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = ToVkPipelineDepthStencilStateCreateInfo(m_State.m_DepthState);
	const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = ToVkPipelineRasterizationStateCreateInfo(m_State.m_RasterizationState);

	const uint32 defaultWidth = 4;
	const uint32 defaultHidth = 4;

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = uint32(vertexInputBindingDescriptions.size());
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = uint32(vertexInputAttributeDescriptions.size());
	vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = ToVkPrimitiveTopology(m_State.m_PrimitiveTopology);
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = float(defaultWidth);
	viewport.height = float(defaultHidth);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent.width = defaultWidth;
	scissor.extent.height = defaultHidth;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = uint32(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = uint32(shaderStageCreateInfos.size());
	graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.data();
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &blendStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = m_VkPipelineLayout;
	graphicsPipelineCreateInfo.renderPass = m_State.m_RenderPass->GetVkRenderPass();
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline vkPipeline = VK_NULL_HANDLE;
	if (vk::API::CreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, vk::Allocator(), &vkPipeline) != VK_SUCCESS) {
		DFAssert(false, "Can't create GraphicsPipeline!");
	}

	DFVkDebugName(vkDevice, vkPipeline, m_Name);

	m_PipelineStateObjects.push_back(PipelineStateObjectSlot(m_State, vkPipeline));
	return vkPipeline;
}