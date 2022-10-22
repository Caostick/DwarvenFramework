#pragma once

#include "VkAPI.h"

#include <DwarfRender/Pipeline.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>

namespace vk {
	class RenderCore;
	class ParameterSet;
}

namespace vk {
	struct PipelineState {
		PipelineState() = default;
		PipelineState(const PipelineState&) = default;
		PipelineState(PipelineState&&) = default;
		PipelineState& operator = (const PipelineState&) = default;
		PipelineState& operator = (PipelineState&&) = default;

		bool operator == (const PipelineState&) const;
		bool operator != (const PipelineState&) const;

		df::BlendState m_BlendState = df::EBlendState::None;
		df::RasterizationState m_RasterizationState = df::ERasterizationState::Fill;
		df::DepthState m_DepthState = df::EDepthStencilState::LessEqual;
		df::EPrimitiveTopology m_PrimitiveTopology = df::EPrimitiveTopology::Triangles;
	};
}

namespace vk {
	class Pipeline : public df::Pipeline {
	public:
		Pipeline(vk::RenderCore& renderCore);

		virtual void SetName(const df::StringView& name) override;

		virtual void SetVertexShader(const df::StringView& code) override;
		virtual void SetFragmentShader(const df::StringView& code) override;

		virtual void SetBlendEnabled(bool value) override;
		virtual void SetColorBlendOp(df::EBlendOp value) override;
		virtual void SetAlphaBlendOp(df::EBlendOp value) override;
		virtual void SetSrcColorBlendFactor(df::EBlendFactor value) override;
		virtual void SetSrcAlphaBlendFactor(df::EBlendFactor value) override;
		virtual void SetDstColorBlendFactor(df::EBlendFactor value) override;
		virtual void SetDstAlphaBlendFactor(df::EBlendFactor value) override;
		virtual void SetBlendState(df::EBlendState value) override;

		virtual void SetDepthTestEnabled(bool value) override;
		virtual void SetDepthWriteEnabled(bool value) override;
		virtual void SetStencilTestEnabled(bool value) override;
		virtual void SetDepthCompareOp(df::EDepthCompareOp value) override;
		virtual void SetStencilOp(df::EStencilOp value) override;
		virtual void SetDepthStencilState(df::EDepthStencilState value) override;

		virtual void SetRasterizerDiscardEnabled(bool value) override;
		virtual void SetPolygonMode(df::EPolygonMode value) override;
		virtual void SetFrontFace(df::EFrontFace value) override;
		virtual void SetCullMode(df::ECullMode value) override;
		virtual void SetRasterizationState(df::ERasterizationState value) override;

		virtual void SetPrimitiveTopology(df::EPrimitiveTopology value) override;

		virtual void SetParameterSet(uint32 index, df::ParameterSet* parameterSet) override;

	public:
		void BuildTest();

	private:
		void CreateShaderModule(const uint32* data, uint32 length, VkShaderModule& shaderModule);

	private:
		vk::RenderCore& m_RenderCore;

		df::String m_Name;

		df::String m_VertexShaderCode;
		df::String m_FragmentShaderCode;

		VkShaderModule m_VkVertexShaderModule;
		VkShaderModule m_VkFragmentShaderModule;

		vk::PipelineState m_State;

		df::Vector<vk::ParameterSet*> m_ParameterSets;
	};
}