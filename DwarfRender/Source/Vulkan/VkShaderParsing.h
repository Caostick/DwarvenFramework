#pragma once

#include "VkParameterSetDefinition.h"
#include "VkVertexAttribute.h"

#include <DwarfRender/BlendState.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>
#include <DwarfRender/PrimitiveTopology.h>

#include <DwarvenCore/Result.h>
#include <DwarvenCore/StringView.h>

namespace vk {
	class RenderCore;
}

namespace vk_shader_parsing {
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

	auto GetTrueFalse(const df::StringView& valueString)->SimpleResult;
	auto GetDepthCompareOp(const df::StringView& valueString)->DepthCompareOpResult;
	auto GetStencilOp(const df::StringView& valueString)->StencilOpResult;
	auto GetPolygonMode(const df::StringView& valueString)->PolygonModeResult;
	auto GetFrontFace(const df::StringView& valueString)->FrontFaceResult;
	auto GetCullMode(const df::StringView& valueString)->CullModeResult;
	auto GetBlendOp(const df::StringView& valueString)->BlendOpResult;
	auto GetBlendFactor(const df::StringView& valueString)->BlendFactorResult;
	auto GetBlendState(const df::StringView& valueString)->BlendStateResult;

	auto FetchDepthTest(const df::StringView& line)->SimpleResult;
	auto FetchDepthWrite(const df::StringView& line)->SimpleResult;
	auto FetchStencilTest(const df::StringView& line)->SimpleResult;
	auto FetchBlendEnable(const df::StringView& line)->SimpleResult;
	auto FetchDepthCompareOp(const df::StringView& line)->DepthCompareOpResult;
	auto FetchStencilOp(const df::StringView& line)->StencilOpResult;
	auto FetchPolygonMode(const df::StringView& line)->PolygonModeResult;
	auto FetchFrontFace(const df::StringView& line)->FrontFaceResult;
	auto FetchCullMode(const df::StringView& line)->CullModeResult;
	auto FetchColorBlendOp(const df::StringView& line)->BlendOpResult;
	auto FetchAlphaBlendOp(const df::StringView& line)->BlendOpResult;
	auto FetchSrcColorBlendFactor(const df::StringView& line)->BlendFactorResult;
	auto FetchBlendState(const df::StringView& line)->BlendStateResult;
	auto FetchDstColorBlendFactor(const df::StringView& line)->BlendFactorResult;
	auto FetchSrcAlphaBlendFactor(const df::StringView& line)->BlendFactorResult;
	auto FetchDstAlphaBlendFactor(const df::StringView& line)->BlendFactorResult;
	auto FetchParameterSet(const df::StringView& line, const vk::RenderCore& renderCore)->ParameterSetResult;
	auto FetchVertexAttribute(const df::StringView& line, const vk::RenderCore& renderCore)->VertexAttributeResult;
}