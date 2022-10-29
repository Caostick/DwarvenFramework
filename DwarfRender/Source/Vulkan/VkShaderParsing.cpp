#include "VkShaderParsing.h"
#include "VkRenderCore.h"

#include <DwarvenCore/StringOperations.h>


auto vk_shader_parsing::GetTrueFalse(const df::StringView& valueString)->SimpleResult {
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

auto vk_shader_parsing::GetDepthCompareOp(const df::StringView& valueString)->DepthCompareOpResult {
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

auto vk_shader_parsing::GetStencilOp(const df::StringView& valueString)->StencilOpResult {
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

auto vk_shader_parsing::GetPolygonMode(const df::StringView& valueString)->PolygonModeResult {
	if (valueString == "Fill") {
		return PolygonModeResult(df::EPolygonMode::Fill);
	} else if (valueString == "Line") {
		return PolygonModeResult(df::EPolygonMode::Line);
	} else if (valueString == "Point") {
		return PolygonModeResult(df::EPolygonMode::Point);
	}

	return PolygonModeResult();
}

auto vk_shader_parsing::GetFrontFace(const df::StringView& valueString)->FrontFaceResult {
	if (valueString == "Clockwise") {
		return FrontFaceResult(df::EFrontFace::Clockwise);
	} else if (valueString == "CounterClockwise") {
		return FrontFaceResult(df::EFrontFace::CounterClockwise);
	}

	return FrontFaceResult();
}

auto vk_shader_parsing::GetCullMode(const df::StringView& valueString)->CullModeResult {
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

auto vk_shader_parsing::GetBlendOp(const df::StringView& valueString)->BlendOpResult {
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

auto vk_shader_parsing::GetBlendFactor(const df::StringView& valueString)->BlendFactorResult {
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

auto vk_shader_parsing::GetBlendState(const df::StringView& valueString)->BlendStateResult {
	if (valueString == "None") {
		return BlendStateResult(df::EBlendState::None);
	} else if (valueString == "Alpha") {
		return BlendStateResult(df::EBlendState::Alpha);
	}

	return BlendStateResult();
}

auto vk_shader_parsing::FetchDepthTest(const df::StringView& line) -> SimpleResult {
	constexpr df::StringView token = "#DepthTest";
	if (!df::EqualPart(line, token)) {
		return SimpleResult();
	}
	return GetTrueFalse(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchDepthWrite(const df::StringView& line) -> SimpleResult {
	constexpr df::StringView token = "#DepthWrite";
	if (!df::EqualPart(line, token)) {
		return SimpleResult();
	}
	return GetTrueFalse(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchStencilTest(const df::StringView& line) -> SimpleResult {
	constexpr df::StringView token = "#StencilTest";
	if (!df::EqualPart(line, token)) {
		return SimpleResult();
	}
	return GetTrueFalse(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchBlendEnable(const df::StringView& line) -> SimpleResult {
	constexpr df::StringView token = "#BlendEnable";
	if (!df::EqualPart(line, token)) {
		return SimpleResult();
	}
	return GetTrueFalse(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchDepthCompareOp(const df::StringView& line) -> DepthCompareOpResult {
	constexpr df::StringView token = "#DepthCompareOp";
	if (!df::EqualPart(line, token)) {
		return DepthCompareOpResult();
	}
	return GetDepthCompareOp(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchStencilOp(const df::StringView& line) -> StencilOpResult {
	constexpr df::StringView token = "#StencilOp";
	if (!df::EqualPart(line, token)) {
		return StencilOpResult();
	}
	return GetStencilOp(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchPolygonMode(const df::StringView& line) -> PolygonModeResult {
	constexpr df::StringView token = "#PolygonMode";
	if (!df::EqualPart(line, token)) {
		return PolygonModeResult();
	}
	return GetPolygonMode(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchFrontFace(const df::StringView& line) -> FrontFaceResult {
	constexpr df::StringView token = "#FrontFace";
	if (!df::EqualPart(line, token)) {
		return FrontFaceResult();
	}
	return GetFrontFace(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchCullMode(const df::StringView& line) -> CullModeResult {
	constexpr df::StringView token = "#CullMode";
	if (!df::EqualPart(line, token)) {
		return CullModeResult();
	}
	return GetCullMode(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchColorBlendOp(const df::StringView& line) -> BlendOpResult {
	constexpr df::StringView token = "#ColorBlendOp";
	if (!df::EqualPart(line, token)) {
		return BlendOpResult();
	}
	return GetBlendOp(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchAlphaBlendOp(const df::StringView& line) -> BlendOpResult {
	constexpr df::StringView token = "#AlphaBlendOp";
	if (!df::EqualPart(line, token)) {
		return BlendOpResult();
	}
	return GetBlendOp(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchSrcColorBlendFactor(const df::StringView& line) -> BlendFactorResult {
	constexpr df::StringView token = "#SrcColorBlendFactor";
	if (!df::EqualPart(line, token)) {
		return BlendFactorResult();
	}
	return GetBlendFactor(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchBlendState(const df::StringView& line) -> BlendStateResult {
	constexpr df::StringView token = "#BlendState";
	if (!df::EqualPart(line, token)) {
		return BlendStateResult();
	}
	return GetBlendState(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchDstColorBlendFactor(const df::StringView& line) -> BlendFactorResult {
	constexpr df::StringView token = "#DstColorBlendFactor";
	if (!df::EqualPart(line, token)) {
		return BlendFactorResult();
	}
	return GetBlendFactor(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchSrcAlphaBlendFactor(const df::StringView& line) -> BlendFactorResult {
	constexpr df::StringView token = "#SrcAlphaBlendFactor";
	if (!df::EqualPart(line, token)) {
		return BlendFactorResult();
	}
	return GetBlendFactor(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchDstAlphaBlendFactor(const df::StringView& line) -> BlendFactorResult {
	constexpr df::StringView token = "#DstAlphaBlendFactor";
	if (!df::EqualPart(line, token)) {
		return BlendFactorResult();
	}
	return GetBlendFactor(df::Trim(df::DropLeft(line, token.length()), " \t"));
}

auto vk_shader_parsing::FetchParameterSet(const df::StringView& line, const vk::RenderCore& renderCore) -> ParameterSetResult {
	constexpr df::StringView token = "#ParameterSet";
	if (!df::EqualPart(line, token)) {
		return ParameterSetResult();
	}
	const df::StringView paramSetName = df::Trim(df::DropLeft(line, token.length()), " \t");

	auto def = renderCore.FindParameterSetDefinition(paramSetName);
	if (def) {
		return ParameterSetResult(def);
	}

	return ParameterSetResult();
}

auto vk_shader_parsing::FetchVertexAttribute(const df::StringView& line, const vk::RenderCore& renderCore) -> VertexAttributeResult {
	constexpr df::StringView token = "#Attribute";
	if (!df::EqualPart(line, token)) {
		return VertexAttributeResult();
	}
	const df::StringView attributeName = df::Trim(df::DropLeft(line, token.length()), " \t");

	const auto attr = renderCore.FindVertexAttribute(attributeName);
	if (attr) {
		return VertexAttributeResult(attr);
	}

	return VertexAttributeResult();
}