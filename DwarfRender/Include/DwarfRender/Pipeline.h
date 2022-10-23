#pragma once

#include <DwarfRender/BlendState.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>
#include <DwarfRender/PrimitiveTopology.h>

#include <DwarvenCore/StringView.h>

namespace df {
	class ParameterSet;
}

namespace df {
	class Pipeline {
	public:
		virtual ~Pipeline() {}

		virtual void DeclareName(const df::StringView& name) = 0;
		virtual void DeclareVertexShader(const df::StringView& code) = 0;
		virtual void DeclareFragmentShader(const df::StringView& code) = 0;
		virtual bool Build() = 0;

		virtual void SetBlendEnabled(bool value) = 0;
		virtual void SetColorBlendOp(EBlendOp value) = 0;
		virtual void SetAlphaBlendOp(EBlendOp value) = 0;
		virtual void SetSrcColorBlendFactor(EBlendFactor value) = 0;
		virtual void SetSrcAlphaBlendFactor(EBlendFactor value) = 0;
		virtual void SetDstColorBlendFactor(EBlendFactor value) = 0;
		virtual void SetDstAlphaBlendFactor(EBlendFactor value) = 0;
		virtual void SetBlendState(EBlendState value) = 0;

		virtual void SetDepthTestEnabled(bool value) = 0;
		virtual void SetDepthWriteEnabled(bool value) = 0;
		virtual void SetStencilTestEnabled(bool value) = 0;
		virtual void SetDepthCompareOp(EDepthCompareOp value) = 0;
		virtual void SetStencilOp(EStencilOp value) = 0;
		virtual void SetDepthStencilState(EDepthStencilState value) = 0;

		virtual void SetRasterizerDiscardEnabled(bool value) = 0;
		virtual void SetPolygonMode(EPolygonMode value) = 0;
		virtual void SetFrontFace(EFrontFace value) = 0;
		virtual void SetCullMode(ECullMode value) = 0;
		virtual void SetRasterizationState(ERasterizationState value) = 0;

		virtual void SetPrimitiveTopology(EPrimitiveTopology value) = 0;
	};
}