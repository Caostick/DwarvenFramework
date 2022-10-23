#pragma once

#include "VkAPI.h"

#include <DwarfRender/Pipeline.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Bitset.h>

namespace vk {
	class RenderCore;
	class RenderPass;
	class ParameterSet;
	class ParameterSetDefinition;

	struct VertexAttribute;
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

		const vk::RenderPass* m_RenderPass = nullptr;
	};
}

namespace vk {
	class Pipeline : public df::Pipeline {
	public:
		Pipeline(vk::RenderCore& renderCore);
		virtual ~Pipeline() override;

		virtual void DeclareName(const df::StringView& name) override;
		virtual void DeclareVertexShader(const df::StringView& code) override;
		virtual void DeclareFragmentShader(const df::StringView& code) override;
		virtual bool Build() override;

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

	public:
		auto GetParameterSetSlot(vk::ParameterSet* parameterSet) const -> int32;
		auto GetPipelineForState(const vk::RenderPass* renderPass)->VkPipeline;

	private:
		auto CreateShaderModule(const uint32* data, uint32 length) -> VkShaderModule;
		auto CreateLayout()->VkPipelineLayout;
		void CreateVertexDescription();


		auto CreatePipelineStateObject()->VkPipeline;

		auto ParseShader(const df::StringView& code) -> df::String;

	private:
		struct PipelineStateObjectSlot {
			PipelineStateObjectSlot() = default;
			PipelineStateObjectSlot(const vk::PipelineState& state, const VkPipeline vkPipeline);

			const vk::PipelineState m_State;
			const VkPipeline m_PipelineStateObject;
		};

		vk::RenderCore& m_RenderCore;

		df::String m_Name;
		df::String m_VertexShaderCode;
		df::String m_FragmentShaderCode;

		VkShaderModule m_VkVertexShaderModule;
		VkShaderModule m_VkFragmentShaderModule;

		vk::PipelineState m_State;

		VkPipelineLayout m_VkPipelineLayout;

		df::Vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
		df::Vector<VkVertexInputBindingDescription> m_BindingDescriptions;

		df::Vector<PipelineStateObjectSlot> m_PipelineStateObjects;
		df::Vector<const vk::ParameterSetDefinition*> m_ParameterSetDefinitions;
		df::Vector<const vk::VertexAttribute*> m_VertexAttributes;
		df::Bitset<64> m_RequiredVertexAttributes;

		bool m_IsBuilt;
	};
}