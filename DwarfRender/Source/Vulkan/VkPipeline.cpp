#include "VkPipeline.h"
#include "VkRenderCore.h"
#include "VkDebug.h"
#include "VkParameterSetDefinition.h"
#include "VkParameterSet.h"

bool vk::PipelineState::operator == (const PipelineState& other) const {
	return
		m_BlendState == other.m_BlendState &&
		m_DepthState == other.m_DepthState &&
		m_RasterizationState == other.m_RasterizationState &&
		m_PrimitiveTopology == other.m_PrimitiveTopology;
}

bool vk::PipelineState::operator != (const PipelineState& other) const {
	return !(operator == (other));
}


vk::Pipeline::Pipeline(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore) 
	, m_VkVertexShaderModule(VK_NULL_HANDLE)
	, m_VkFragmentShaderModule(VK_NULL_HANDLE) {

}

void vk::Pipeline::SetName(const df::StringView& name) {
	m_Name = name;
}

void vk::Pipeline::SetVertexShader(const df::StringView& code) {
	m_VertexShaderCode = df::String(code);
}

void vk::Pipeline::SetFragmentShader(const df::StringView& code) {
	m_FragmentShaderCode = df::String(code);
}

void vk::Pipeline::SetBlendEnabled(bool value) {
	m_State.m_BlendState.m_BlendEnable = value;
}

void vk::Pipeline::SetColorBlendOp(df::EBlendOp value) {
	m_State.m_BlendState.m_ColorBlendOp = value;
}

void vk::Pipeline::SetAlphaBlendOp(df::EBlendOp value) {
	m_State.m_BlendState.m_AlphaBlendOp = value;
}

void vk::Pipeline::SetSrcColorBlendFactor(df::EBlendFactor value) {
	m_State.m_BlendState.m_SrcColorBlendFactor = value;
}

void vk::Pipeline::SetSrcAlphaBlendFactor(df::EBlendFactor value) {
	m_State.m_BlendState.m_SrcAlphaBlendFactor = value;
}

void vk::Pipeline::SetDstColorBlendFactor(df::EBlendFactor value) {
	m_State.m_BlendState.m_DstColorBlendFactor = value;
}

void vk::Pipeline::SetDstAlphaBlendFactor(df::EBlendFactor value) {
	m_State.m_BlendState.m_DstAlphaBlendFactor = value;
}

void vk::Pipeline::SetBlendState(df::EBlendState value) {
	m_State.m_BlendState = value;
}

void vk::Pipeline::SetDepthTestEnabled(bool value) {
	m_State.m_DepthState.m_DepthTestEnable = value;
}

void vk::Pipeline::SetDepthWriteEnabled(bool value) {
	m_State.m_DepthState.m_DepthWriteEnable = value;
}

void vk::Pipeline::SetStencilTestEnabled(bool value) {
	m_State.m_DepthState.m_StencilTestEnable = value;
}

void vk::Pipeline::SetDepthCompareOp(df::EDepthCompareOp value) {
	m_State.m_DepthState.m_DepthCompareOp = value;
}

void vk::Pipeline::SetStencilOp(df::EStencilOp value) {
	m_State.m_DepthState.m_StencilOp = value;
}

void vk::Pipeline::SetDepthStencilState(df::EDepthStencilState value) {
	m_State.m_DepthState = value;
}

void vk::Pipeline::SetRasterizerDiscardEnabled(bool value) {
	m_State.m_RasterizationState.m_RasterizerDiscardEnable = value;
}

void vk::Pipeline::SetPolygonMode(df::EPolygonMode value) {
	m_State.m_RasterizationState.m_PolygonMode = value;
}

void vk::Pipeline::SetFrontFace(df::EFrontFace value) {
	m_State.m_RasterizationState.m_FrontFace = value;
}

void vk::Pipeline::SetCullMode(df::ECullMode value) {
	m_State.m_RasterizationState.m_CullMode = value;
}

void vk::Pipeline::SetRasterizationState(df::ERasterizationState value) {
	m_State.m_RasterizationState = value;
}

void vk::Pipeline::SetPrimitiveTopology(df::EPrimitiveTopology value) {
	m_State.m_PrimitiveTopology = value;
}

void vk::Pipeline::SetParameterSet(uint32 index, df::ParameterSet* parameterSet) {
	if (m_ParameterSets.size() <= index) {
		m_ParameterSets.resize(index + 1);
	}

	m_ParameterSets[index] = static_cast<vk::ParameterSet*>(parameterSet);
}

void vk::Pipeline::SetVertexShaderSpirV(const uint32* data, uint32 length) {
	CreateShaderModule(data, length, m_VkVertexShaderModule);
}

void vk::Pipeline::SetFragmentShaderSpirV(const uint32* data, uint32 length) {
	CreateShaderModule(data, length, m_VkFragmentShaderModule);
}

void vk::Pipeline::CreateShaderModule(const uint32* data, uint32 length, VkShaderModule& shaderModule) {
	if (shaderModule != VK_NULL_HANDLE) {
		m_RenderCore.RemoveShaderModule(shaderModule);
	}

	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = length * sizeof(uint32);
	shaderModuleCreateInfo.pCode = data;

	if (vk::API::CreateShaderModule(vkDevice, &shaderModuleCreateInfo, vk::Allocator(), &(shaderModule)) != VK_SUCCESS) {
		DFAssert(false, "Can't create ShaderModule!");
	}

	DFVkDebugName(vkDevice, shaderModule, m_Name);
}