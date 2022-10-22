#include "VkShaderFileIncluder.h"
#include "VkParameterSetDefinition.h"
#include "VkShaderCompileInfo.h"
#include "VkRenderCore.h"

#include <DwarfRender/BlendState.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>
#include <DwarfRender/PrimitiveTopology.h>
#include <DwarfRender/VertexAttribute.h>

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/New.h>

namespace {
	using SimpleResult = df::ResultNoError<bool>;
	using ParameterSetResult = df::ResultNoError<const vk::ParameterSetDefinition*>;
	using VertexAttributeResult = df::ResultNoError<const vk::VertexAttribute*>;
	using DepthCompareOpResult = df::ResultNoError<df::EDepthCompareOp>;
	using StencilOpResult = df::ResultNoError<df::EStencilOp>;
	using PolygonModeResult = df::ResultNoError<df::EPolygonMode>;
	using FrontFaceResult = df::ResultNoError<df::EFrontFace>;
	using CullModeResult = df::ResultNoError<df::ECullMode>;
	using BlendOpResult = df::ResultNoError<df::EBlendOp>;
	using BlendFactorResult = df::ResultNoError<df::EBlendFactor>;
	using BlendStateResult = df::ResultNoError<df::EBlendState>;
	
	auto GetTrueFalse(const df::StringView& valueString)->SimpleResult {
		if (valueString == "1"
			|| valueString == "True"
			|| valueString == "On"
			|| valueString == "Enable"
			|| valueString == "Enabled") {
			return SimpleResult(true);
		} else if (valueString == "0"
			|| valueString == "False"
			|| valueString == "Off"
			|| valueString == "Disable"
			|| valueString == "Disabled") {
			return SimpleResult(false);
		}

		return SimpleResult();
	}

	auto GetDepthCompareOp(const df::StringView& valueString)->DepthCompareOpResult {
		if (valueString == "Never") {
			return DepthCompareOpResult(df::EDepthCompareOp::Never);
		} else if (valueString == "Equal") {
			return DepthCompareOpResult(df::EDepthCompareOp::Equal);
		} else if (valueString == "NotEqual") {
			return DepthCompareOpResult(df::EDepthCompareOp::NotEqual);
		} else if (valueString == "Greater") {
			return DepthCompareOpResult(df::EDepthCompareOp::Greater);
		} else if (valueString == "GreaterEqual") {
			return DepthCompareOpResult(df::EDepthCompareOp::GreaterEqual);
		} else if (valueString == "Less") {
			return DepthCompareOpResult(df::EDepthCompareOp::Less);
		} else if (valueString == "LessEqual") {
			return DepthCompareOpResult(df::EDepthCompareOp::LessEqual);
		} else if (valueString == "Always") {
			return DepthCompareOpResult(df::EDepthCompareOp::Always);
		}

		return DepthCompareOpResult();
	}

	auto GetStencilOp(const df::StringView& valueString)->StencilOpResult {
		if (valueString == "Keep") {
			return StencilOpResult(df::EStencilOp::Keep);
		} else if (valueString == "Zero") {
			return StencilOpResult(df::EStencilOp::Zero);
		} else if (valueString == "Replace") {
			return StencilOpResult(df::EStencilOp::Replace);
		} else if (valueString == "Increment") {
			return StencilOpResult(df::EStencilOp::Increment);
		} else if (valueString == "Decrement") {
			return StencilOpResult(df::EStencilOp::Decrement);
		} else if (valueString == "Invert") {
			return StencilOpResult(df::EStencilOp::Invert);
		}

		return StencilOpResult();
	}

	auto GetPolygonMode(const df::StringView& valueString)->PolygonModeResult {
		if (valueString == "Fill") {
			return PolygonModeResult(df::EPolygonMode::Fill);
		} else if (valueString == "Line") {
			return PolygonModeResult(df::EPolygonMode::Line);
		} else if (valueString == "Point") {
			return PolygonModeResult(df::EPolygonMode::Point);
		}

		return PolygonModeResult();
	}

	auto GetFrontFace(const df::StringView& valueString)->FrontFaceResult {
		if (valueString == "Clockwise") {
			return FrontFaceResult(df::EFrontFace::Clockwise);
		} else if (valueString == "CounterClockwise") {
			return FrontFaceResult(df::EFrontFace::CounterClockwise);
		}

		return FrontFaceResult();
	}

	auto GetCullMode(const df::StringView& valueString)->CullModeResult {
		if (valueString == "None") {
			return CullModeResult(df::ECullMode::None);
		} else if (valueString == "Front") {
			return CullModeResult(df::ECullMode::Front);
		} else if (valueString == "Back") {
			return CullModeResult(df::ECullMode::Back);
		} else if (valueString == "FrontAndBack") {
			return CullModeResult(df::ECullMode::FrontAndBack);
		}

		return CullModeResult();
	}

	auto GetBlendOp(const df::StringView& valueString)->BlendOpResult {
		if (valueString == "Add") {
			return BlendOpResult(df::EBlendOp::Add);
		} else if (valueString == "Subtract") {
			return BlendOpResult(df::EBlendOp::Subtract);
		} else if (valueString == "ReverseSubtract") {
			return BlendOpResult(df::EBlendOp::ReverseSubtract);
		} else if (valueString == "Min") {
			return BlendOpResult(df::EBlendOp::Min);
		} else if (valueString == "Max") {
			return BlendOpResult(df::EBlendOp::Max);
		}

		return BlendOpResult();
	}

	auto GetBlendFactor(const df::StringView& valueString)->BlendFactorResult {
		if (valueString == "Zero") {
			return BlendFactorResult(df::EBlendFactor::Zero);
		} else if (valueString == "One") {
			return BlendFactorResult(df::EBlendFactor::One);
		} else if (valueString == "SrcColor") {
			return BlendFactorResult(df::EBlendFactor::SrcColor);
		} else if (valueString == "OneMinusSrcColor") {
			return BlendFactorResult(df::EBlendFactor::OneMinusSrcColor);
		} else if (valueString == "DstColor") {
			return BlendFactorResult(df::EBlendFactor::DstColor);
		} else if (valueString == "OneMinusDstColor") {
			return BlendFactorResult(df::EBlendFactor::OneMinusDstColor);
		} else if (valueString == "SrcAlpha") {
			return BlendFactorResult(df::EBlendFactor::SrcAlpha);
		} else if (valueString == "OneMinusSrcAlpha") {
			return BlendFactorResult(df::EBlendFactor::OneMinusSrcAlpha);
		} else if (valueString == "DstAlpha") {
			return BlendFactorResult(df::EBlendFactor::DstAlpha);
		} else if (valueString == "OneMinusDstAlpha") {
			return BlendFactorResult(df::EBlendFactor::OneMinusDstAlpha);
		}

		return BlendFactorResult();
	}

	auto GetBlendState(const df::StringView& valueString)->BlendStateResult {
		if (valueString == "None") {
			return BlendStateResult(df::EBlendState::None);
		}
		else if (valueString == "Alpha") {
			return BlendStateResult(df::EBlendState::Alpha);
		}

		return BlendStateResult();
	}

	auto FetchDepthTest(const df::StringView& include) -> SimpleResult {
		constexpr df::StringView token = "DepthTest:";
		if (!df::EqualPart(include, token)) {
			return SimpleResult();
		}
		return GetTrueFalse(df::DropLeft(include, token.length()));
	}

	auto FetchDepthWrite(const df::StringView& include) -> SimpleResult {
		constexpr df::StringView token = "DepthWrite:";
		if (!df::EqualPart(include, token)) {
			return SimpleResult();
		}
		return GetTrueFalse(df::DropLeft(include, token.length()));
	}

	auto FetchStencilTest(const df::StringView& include) -> SimpleResult {
		constexpr df::StringView token = "StencilTest:";
		if (!df::EqualPart(include, token)) {
			return SimpleResult();
		}
		return GetTrueFalse(df::DropLeft(include, token.length()));
	}

	auto FetchBlendEnable(const df::StringView& include) -> SimpleResult {
		constexpr df::StringView token = "BlendEnable:";
		if (!df::EqualPart(include, token)) {
			return SimpleResult();
		}
		return GetTrueFalse(df::DropLeft(include, token.length()));
	}

	auto FetchDepthCompareOp(const df::StringView& include) -> DepthCompareOpResult {
		constexpr df::StringView token = "DepthCompareOp:";
		if (!df::EqualPart(include, token)) {
			return DepthCompareOpResult();
		}
		return GetDepthCompareOp(df::DropLeft(include, token.length()));
	}

	auto FetchStencilOp(const df::StringView& include) -> StencilOpResult {
		constexpr df::StringView token = "StencilOp:";
		if (!df::EqualPart(include, token)) {
			return StencilOpResult();
		}
		return GetStencilOp(df::DropLeft(include, token.length()));
	}

	auto FetchPolygonMode(const df::StringView& include) -> PolygonModeResult {
		constexpr df::StringView token = "PolygonMode:";
		if (!df::EqualPart(include, token)) {
			return PolygonModeResult();
		}
		return GetPolygonMode(df::DropLeft(include, token.length()));
	}

	auto FetchFrontFace(const df::StringView& include) -> FrontFaceResult {
		constexpr df::StringView token = "FrontFace:";
		if (!df::EqualPart(include, token)) {
			return FrontFaceResult();
		}
		return GetFrontFace(df::DropLeft(include, token.length()));
	}

	auto FetchCullMode(const df::StringView& include) -> CullModeResult {
		constexpr df::StringView token = "CullMode:";
		if (!df::EqualPart(include, token)) {
			return CullModeResult();
		}
		return GetCullMode(df::DropLeft(include, token.length()));
	}

	auto FetchColorBlendOp(const df::StringView& include) -> BlendOpResult {
		constexpr df::StringView token = "ColorBlendOp:";
		if (!df::EqualPart(include, token)) {
			return BlendOpResult();
		}
		return GetBlendOp(df::DropLeft(include, token.length()));
	}

	auto FetchAlphaBlendOp(const df::StringView& include) -> BlendOpResult {
		constexpr df::StringView token = "AlphaBlendOp:";
		if (!df::EqualPart(include, token)) {
			return BlendOpResult();
		}
		return GetBlendOp(df::DropLeft(include, token.length()));
	}

	auto FetchSrcColorBlendFactor(const df::StringView& include) -> BlendFactorResult {
		constexpr df::StringView token = "SrcColorBlendFactor:";
		if (!df::EqualPart(include, token)) {
			return BlendFactorResult();
		}
		return GetBlendFactor(df::DropLeft(include, token.length()));
	}

	auto FetchBlendState(const df::StringView& include) -> BlendStateResult {
		constexpr df::StringView token = "BlendState:";
		if (!df::EqualPart(include, token)) {
			return BlendStateResult();
		}
		return GetBlendState(df::DropLeft(include, token.length()));
	}

	auto FetchDstColorBlendFactor(const df::StringView& include) -> BlendFactorResult {
		constexpr df::StringView token = "DstColorBlendFactor:";
		if (!df::EqualPart(include, token)) {
			return BlendFactorResult();
		}
		return GetBlendFactor(df::DropLeft(include, token.length()));
	}

	auto FetchSrcAlphaBlendFactor(const df::StringView& include) -> BlendFactorResult {
		constexpr df::StringView token = "SrcAlphaBlendFactor:";
		if (!df::EqualPart(include, token)) {
			return BlendFactorResult();
		}
		return GetBlendFactor(df::DropLeft(include, token.length()));
	}

	auto FetchDstAlphaBlendFactor(const df::StringView& include) -> BlendFactorResult {
		constexpr df::StringView token = "DstAlphaBlendFactor:";
		if (!df::EqualPart(include, token)) {
			return BlendFactorResult();
		}
		return GetBlendFactor(df::DropLeft(include, token.length()));
	}

	auto FetchParameterSet(const df::StringView& include, const vk::RenderCore& renderCore) -> ParameterSetResult {
		constexpr df::StringView token = "ParameterSet:";
		if (!df::EqualPart(include, token)) {
			return ParameterSetResult();
		}
		const df::StringView paramSetName = df::DropLeft(include, token.length());

		auto def = renderCore.FindParameterSetDefinition(paramSetName);
		if (def) {
			return ParameterSetResult(def);
		}

		return ParameterSetResult();
	}

	auto FetchVertexAttribute(const vk::RenderCore& renderCore, const df::StringView& include) -> VertexAttributeResult {
		constexpr df::StringView token = "Attribute:";
		if (!df::EqualPart(include, token)) {
			return VertexAttributeResult();
		}
		const df::StringView attributeName = df::DropLeft(include, token.length());

		const auto attr = renderCore.FindVertexAttribute(attributeName);
		if (attr) {
			return VertexAttributeResult(attr);
		}

		return VertexAttributeResult();
	}
}

vk::ShaderFileIncluder::ShaderFileIncluder(
	const vk::RenderCore& renderCore,
	vk::ShaderCompileInfo& info)
	: m_RenderCore(renderCore)
	, m_CompileInfo(info) {}

vk::ShaderFileIncluder::~ShaderFileIncluder() {
	for (auto str : m_TempStrings) {
		DFDelete str;
	}
}

glslang::TShader::Includer::IncludeResult* vk::ShaderFileIncluder::includeLocal(
	const char* headerName,
	const char* /*includerName*/,
	size_t /*inclusionDepth*/) {

	const df::StringView includeName = df::StringView(headerName);

	if (const auto vertexAttribule = FetchVertexAttribute(m_RenderCore, includeName)) {
		const auto attr = vertexAttribule.Get();

		const auto& code = attr->m_ShaderString;
		return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);

	} else if (const auto parameterSet = FetchParameterSet(includeName, m_RenderCore)) {
		const auto paramsDefinition = parameterSet.Get();
		if (df::AddUnique(m_CompileInfo.m_ParameterSets, paramsDefinition)) {
			const uint32 idx = uint32(df::FindElement(m_CompileInfo.m_ParameterSets, paramsDefinition));

			auto* code = DFNew df::String;
			m_TempStrings.push_back(code);

			*code = paramsDefinition->MakeShaderSnippet(idx);
			return DFNew IncludeResult(headerName, code->c_str(), code->size(), nullptr);
		} else {
			const auto& code = m_RenderCore.GetShaderInclude("");
			return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);
		}

	} else if (const auto depthTest = FetchDepthTest(includeName)) {
		m_CompileInfo.m_DepthState.m_DepthTestEnable = depthTest.Get();

	} else if (const auto depthWrite = FetchDepthWrite(includeName)) {
		m_CompileInfo.m_DepthState.m_DepthWriteEnable = depthWrite.Get();

	} else if (const auto stencilTest = FetchStencilTest(includeName)) {
		m_CompileInfo.m_DepthState.m_StencilTestEnable = stencilTest.Get();

	} else if (const auto depthCompareOp = FetchDepthCompareOp(includeName)) {
		m_CompileInfo.m_DepthState.m_DepthCompareOp = depthCompareOp.Get();

	} else if (const auto stencilOp = FetchStencilOp(includeName)) {
		m_CompileInfo.m_DepthState.m_StencilOp = stencilOp.Get();

	} else if (const auto polygonMode = FetchPolygonMode(includeName)) {
		m_CompileInfo.m_RasterizationState.m_PolygonMode = polygonMode.Get();

	} else if (const auto frontFace = FetchFrontFace(includeName)) {
		m_CompileInfo.m_RasterizationState.m_FrontFace = frontFace.Get();

	} else if (const auto cullMode = FetchCullMode(includeName)) {
		m_CompileInfo.m_RasterizationState.m_CullMode = cullMode.Get();

	} else if (const auto blendEnable = FetchBlendEnable(includeName)) {
		m_CompileInfo.m_BlendState.m_BlendEnable = blendEnable.Get();

	} else if (const auto colorBlendOp = FetchColorBlendOp(includeName)) {
		m_CompileInfo.m_BlendState.m_ColorBlendOp = colorBlendOp.Get();

	} else if (const auto alphaBlendOp = FetchAlphaBlendOp(includeName)) {
		m_CompileInfo.m_BlendState.m_AlphaBlendOp = alphaBlendOp.Get();

	} else if (const auto srcColorBlendFactor = FetchSrcColorBlendFactor(includeName)) {
		m_CompileInfo.m_BlendState.m_SrcColorBlendFactor = srcColorBlendFactor.Get();

	} else if (const auto dstColorBlendFactor = FetchDstColorBlendFactor(includeName)) {
		m_CompileInfo.m_BlendState.m_DstColorBlendFactor = dstColorBlendFactor.Get();

	} else if (const auto srcAlphaBlendFactor = FetchSrcAlphaBlendFactor(includeName)) {
		m_CompileInfo.m_BlendState.m_SrcAlphaBlendFactor = srcAlphaBlendFactor.Get();

	} else if (const auto dstAlphaBlendFactor = FetchDstAlphaBlendFactor(includeName)) {
		m_CompileInfo.m_BlendState.m_DstAlphaBlendFactor = dstAlphaBlendFactor.Get();

	} else if (const auto blendState = FetchBlendState(includeName)) {
		m_CompileInfo.m_BlendState = blendState.Get();

	} else {
		const auto& code = m_RenderCore.GetShaderInclude(headerName);
		return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);
	}

	return DFNew IncludeResult(headerName, "", 0, nullptr);
}

void vk::ShaderFileIncluder::releaseInclude(IncludeResult* result) {
	DFDelete result;
}