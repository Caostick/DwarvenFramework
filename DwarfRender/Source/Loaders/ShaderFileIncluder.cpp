#include "ShaderFileIncluder.h"

#include <DwarfRender/ParamSetDefinition.h>
#include <DwarfRender/ParamSetDefinitionManager.h>
#include <DwarfRender/Resources/ShaderIncludeResource.h>
#include <DwarfRender/Loaders/ShaderCompiler.h>
#include <DwarfRender/VertexAttributeRegistry.h>

#include <DwarfResources/ResourceManager.h>

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/New.h>

namespace {
	df::Vector<df::String> TmpAttributeLines;

	using SimpleResult = df::ResultNoError<bool>;
	using ParameterSetResult = df::ResultNoError<const rf::ParamSetDefinition*>;
	using VertexAttributeResult = df::ResultNoError<int32>;
	using DepthCompareOpResult = df::ResultNoError<rf::EDepthCompareOp>;
	using StencilOpResult = df::ResultNoError<rf::EStencilOp>;
	using PolygonModeResult = df::ResultNoError<rf::EPolygonMode>;
	using FrontFaceResult = df::ResultNoError<rf::EFrontFace>;
	using CullModeResult = df::ResultNoError<rf::ECullMode>;
	using BlendOpResult = df::ResultNoError<rf::EBlendOp>;
	using BlendFactorResult = df::ResultNoError<rf::EBlendFactor>;
	using BlendStateResult = df::ResultNoError<rf::EBlendState>;
	
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
			return DepthCompareOpResult(rf::EDepthCompareOp::Never);
		} else if (valueString == "Equal") {
			return DepthCompareOpResult(rf::EDepthCompareOp::Equal);
		} else if (valueString == "NotEqual") {
			return DepthCompareOpResult(rf::EDepthCompareOp::NotEqual);
		} else if (valueString == "Greater") {
			return DepthCompareOpResult(rf::EDepthCompareOp::Greater);
		} else if (valueString == "GreaterEqual") {
			return DepthCompareOpResult(rf::EDepthCompareOp::GreaterEqual);
		} else if (valueString == "Less") {
			return DepthCompareOpResult(rf::EDepthCompareOp::Less);
		} else if (valueString == "LessEqual") {
			return DepthCompareOpResult(rf::EDepthCompareOp::LessEqual);
		} else if (valueString == "Always") {
			return DepthCompareOpResult(rf::EDepthCompareOp::Always);
		}

		return DepthCompareOpResult();
	}

	auto GetStencilOp(const df::StringView& valueString)->StencilOpResult {
		if (valueString == "Keep") {
			return StencilOpResult(rf::EStencilOp::Keep);
		} else if (valueString == "Zero") {
			return StencilOpResult(rf::EStencilOp::Zero);
		} else if (valueString == "Replace") {
			return StencilOpResult(rf::EStencilOp::Replace);
		} else if (valueString == "Increment") {
			return StencilOpResult(rf::EStencilOp::Increment);
		} else if (valueString == "Decrement") {
			return StencilOpResult(rf::EStencilOp::Decrement);
		} else if (valueString == "Invert") {
			return StencilOpResult(rf::EStencilOp::Invert);
		}

		return StencilOpResult();
	}

	auto GetPolygonMode(const df::StringView& valueString)->PolygonModeResult {
		if (valueString == "Fill") {
			return PolygonModeResult(rf::EPolygonMode::Fill);
		} else if (valueString == "Line") {
			return PolygonModeResult(rf::EPolygonMode::Line);
		} else if (valueString == "Point") {
			return PolygonModeResult(rf::EPolygonMode::Point);
		}

		return PolygonModeResult();
	}

	auto GetFrontFace(const df::StringView& valueString)->FrontFaceResult {
		if (valueString == "Clockwise") {
			return FrontFaceResult(rf::EFrontFace::Clockwise);
		} else if (valueString == "CounterClockwise") {
			return FrontFaceResult(rf::EFrontFace::CounterClockwise);
		}

		return FrontFaceResult();
	}

	auto GetCullMode(const df::StringView& valueString)->CullModeResult {
		if (valueString == "None") {
			return CullModeResult(rf::ECullMode::None);
		} else if (valueString == "Front") {
			return CullModeResult(rf::ECullMode::Front);
		} else if (valueString == "Back") {
			return CullModeResult(rf::ECullMode::Back);
		} else if (valueString == "FrontAndBack") {
			return CullModeResult(rf::ECullMode::FrontAndBack);
		}

		return CullModeResult();
	}

	auto GetBlendOp(const df::StringView& valueString)->BlendOpResult {
		if (valueString == "Add") {
			return BlendOpResult(rf::EBlendOp::Add);
		} else if (valueString == "Subtract") {
			return BlendOpResult(rf::EBlendOp::Subtract);
		} else if (valueString == "ReverseSubtract") {
			return BlendOpResult(rf::EBlendOp::ReverseSubtract);
		} else if (valueString == "Min") {
			return BlendOpResult(rf::EBlendOp::Min);
		} else if (valueString == "Max") {
			return BlendOpResult(rf::EBlendOp::Max);
		}

		return BlendOpResult();
	}

	auto GetBlendFactor(const df::StringView& valueString)->BlendFactorResult {
		if (valueString == "Zero") {
			return BlendFactorResult(rf::EBlendFactor::Zero);
		} else if (valueString == "One") {
			return BlendFactorResult(rf::EBlendFactor::One);
		} else if (valueString == "SrcColor") {
			return BlendFactorResult(rf::EBlendFactor::SrcColor);
		} else if (valueString == "OneMinusSrcColor") {
			return BlendFactorResult(rf::EBlendFactor::OneMinusSrcColor);
		} else if (valueString == "DstColor") {
			return BlendFactorResult(rf::EBlendFactor::DstColor);
		} else if (valueString == "OneMinusDstColor") {
			return BlendFactorResult(rf::EBlendFactor::OneMinusDstColor);
		} else if (valueString == "SrcAlpha") {
			return BlendFactorResult(rf::EBlendFactor::SrcAlpha);
		} else if (valueString == "OneMinusSrcAlpha") {
			return BlendFactorResult(rf::EBlendFactor::OneMinusSrcAlpha);
		} else if (valueString == "DstAlpha") {
			return BlendFactorResult(rf::EBlendFactor::DstAlpha);
		} else if (valueString == "OneMinusDstAlpha") {
			return BlendFactorResult(rf::EBlendFactor::OneMinusDstAlpha);
		}

		return BlendFactorResult();
	}

	auto GetBlendState(const df::StringView& valueString)->BlendStateResult {
		if (valueString == "None") {
			return BlendStateResult(rf::EBlendState::None);
		}
		else if (valueString == "Alpha") {
			return BlendStateResult(rf::EBlendState::Alpha);
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

	auto FetchParameterSet(const df::StringView& include, const rf::ParamSetDefinitionManager& paramSetDefinitionManager) -> ParameterSetResult {
		constexpr df::StringView token = "ParamSet:";
		if (!df::EqualPart(include, token)) {
			return ParameterSetResult();
		}
		const df::StringView paramSetName = df::DropLeft(include, token.length());

		if (paramSetName == "Global") {
			return ParameterSetResult(paramSetDefinitionManager.GetGlobalDefinition());
		} else if (paramSetName == "View") {
			return ParameterSetResult(paramSetDefinitionManager.GetViewDefinition());
		} else if (paramSetName.size() >= 5 && (df::TakeLeft(paramSetName, 5) == "Pass\\")) {
			const df::StringView passName = df::DropLeft(paramSetName, 5);
			const auto paramsDefinition = paramSetDefinitionManager.GetDefinitionForPass(passName);
			if (paramsDefinition) {
				return ParameterSetResult(paramsDefinition);
			}
		} else if (paramSetName.size() >= 9 && (df::TakeLeft(paramSetName, 9) == "Material\\")) {
			const df::StringView materialName = df::DropLeft(paramSetName, 9);
			const auto paramsDefinition = paramSetDefinitionManager.GetDefinitionForMaterial(materialName);
			if (paramsDefinition) {
				return ParameterSetResult(paramsDefinition);
			}
		}

		return ParameterSetResult();
	}

	auto FetchVertexAttribute(const df::StringView& include) -> VertexAttributeResult {
		constexpr df::StringView token = "Attribute:";
		if (!df::EqualPart(include, token)) {
			return VertexAttributeResult();
		}
		const df::StringView attributeName = df::DropLeft(include, token.length());

		const int32 idx = rf::VertexAttributeRegistry::FindByName(attributeName);
		if (idx != -1) {
			if (TmpAttributeLines.empty()) {
				TmpAttributeLines.resize(rf::VertexAttributeRegistry::TypeCount);
			}
			if (TmpAttributeLines[idx].empty()) {
				const auto& entry = rf::VertexAttributeRegistry::Entries[idx];
				const auto strLocation = std::to_string(idx);
				const df::String strType = rf::ToShaderInOutString(rf::ToShaderInOutType(entry.Format));
				const df::String strName = entry.Name;
				TmpAttributeLines[idx] = df::String("layout(location = ") + strLocation + ") in " + strType + df::String(" ") + strName + df::String(";\n");
			}

			return VertexAttributeResult(idx);
		}

		return VertexAttributeResult();
	}
}

rf::ShaderFileIncluder::ShaderFileIncluder(
	const df::ResourceManager& resourceManager,
	const rf::ParamSetDefinitionManager& paramSetDefinitionManager,
	rf::ShaderCompileInfo& info)
	: m_ResourceManager(resourceManager)
	, m_ParamSetDefinitionManager(paramSetDefinitionManager)
	, m_ShaderCompileInfo(info) {}

glslang::TShader::Includer::IncludeResult* rf::ShaderFileIncluder::includeLocal(
	const char* headerName,
	const char* /*includerName*/,
	size_t /*inclusionDepth*/) {

	const df::StringView includeName = df::StringView(headerName);

	if (const auto vertexAttribule = FetchVertexAttribute(includeName)) {
		const auto idx = vertexAttribule.Get();

		rf::VertexAttributeLocation location;
		location.m_AttributeIndex = idx;
		location.m_ShaderLocation = idx;
		m_ShaderCompileInfo.m_Attributes.emplace_back(location);
		const auto& code = TmpAttributeLines[idx];
		return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);

	} else if (const auto parameterSet = FetchParameterSet(includeName, m_ParamSetDefinitionManager)) {
		const auto paramsDefinition = parameterSet.Get();

		const auto& code = paramsDefinition->GetShaderSnippet();
		m_ShaderCompileInfo.m_ParameterSets.push_back(paramsDefinition);
		return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);

	} else if (const auto depthTest = FetchDepthTest(includeName)) {
		m_ShaderCompileInfo.m_DepthState.m_DepthTestEnable = depthTest.Get();

	} else if (const auto depthWrite = FetchDepthWrite(includeName)) {
		m_ShaderCompileInfo.m_DepthState.m_DepthWriteEnable = depthWrite.Get();

	} else if (const auto stencilTest = FetchStencilTest(includeName)) {
		m_ShaderCompileInfo.m_DepthState.m_StencilTestEnable = stencilTest.Get();

	} else if (const auto depthCompareOp = FetchDepthCompareOp(includeName)) {
		m_ShaderCompileInfo.m_DepthState.m_DepthCompareOp = depthCompareOp.Get();

	} else if (const auto stencilOp = FetchStencilOp(includeName)) {
		m_ShaderCompileInfo.m_DepthState.m_StencilOp = stencilOp.Get();

	} else if (const auto polygonMode = FetchPolygonMode(includeName)) {
		m_ShaderCompileInfo.m_RasterizationState.m_PolygonMode = polygonMode.Get();

	} else if (const auto frontFace = FetchFrontFace(includeName)) {
		m_ShaderCompileInfo.m_RasterizationState.m_FrontFace = frontFace.Get();

	} else if (const auto cullMode = FetchCullMode(includeName)) {
		m_ShaderCompileInfo.m_RasterizationState.m_CullMode = cullMode.Get();

	} else if (const auto blendEnable = FetchBlendEnable(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_BlendEnable = blendEnable.Get();

	} else if (const auto colorBlendOp = FetchColorBlendOp(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_ColorBlendOp = colorBlendOp.Get();

	} else if (const auto alphaBlendOp = FetchAlphaBlendOp(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_AlphaBlendOp = alphaBlendOp.Get();

	} else if (const auto srcColorBlendFactor = FetchSrcColorBlendFactor(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_SrcColorBlendFactor = srcColorBlendFactor.Get();

	} else if (const auto dstColorBlendFactor = FetchDstColorBlendFactor(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_DstColorBlendFactor = dstColorBlendFactor.Get();

	} else if (const auto srcAlphaBlendFactor = FetchSrcAlphaBlendFactor(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_SrcAlphaBlendFactor = srcAlphaBlendFactor.Get();

	} else if (const auto dstAlphaBlendFactor = FetchDstAlphaBlendFactor(includeName)) {
		m_ShaderCompileInfo.m_BlendState.m_DstAlphaBlendFactor = dstAlphaBlendFactor.Get();

	} else if (const auto blendState = FetchBlendState(includeName)) {
		m_ShaderCompileInfo.m_BlendState = blendState.Get();

	} else {
		auto resource = m_ResourceManager.GetResource<rf::ShaderIncludeResource>(headerName);
		if (resource) {
			const auto& code = resource->GetCode();
			return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);
		}
	}

	return DFNew IncludeResult(headerName, "", 0, nullptr);
}

void rf::ShaderFileIncluder::releaseInclude(IncludeResult* result) {
	DFDelete result;
}
