#pragma once

#include <DwarfRender/BlendState.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>
#include <DwarfRender/PrimitiveTopology.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>

namespace df {
	class ParameterSet;
	struct VertexAttribute;
}

namespace df {
	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() {}

		virtual void SetName(const StringView& name) = 0;
		virtual void SetupVertexShader(const Vector<uint32>& bytecode) = 0;
		virtual void SetupFragmentShader(const Vector<uint32>& bytecode) = 0;
		virtual void SetupVertexAttributes(const Vector<String>& vertexAttributes) = 0;
		virtual void SetupParameterSets(const Vector<String>& parameterSets) = 0;
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