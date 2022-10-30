#pragma once

#include "VkAPI.h"

#include <DwarfRender/VertexAttribute.h>

namespace vk {
	enum class EShaderInOutType {
		Float,
		Vec2,
		Vec3,
		Vec4,

		Int,
		IVec2,
		IVec3,
		IVec4,

		UInt,
		UIVec2,
		UIVec3,
		UIVec4,

		COUNT
	};

	auto ToShaderInOutType(df::EVertexAttributeFormat format)->EShaderInOutType;
	auto ToDataStride(df::EVertexAttributeFormat format)->uint32;
	auto ToVkFormat(df::EVertexAttributeFormat format)->VkFormat;
	auto ToAttributeSize(df::EVertexAttributeFormat format)->uint32;
	auto ToAttributeSize(EShaderInOutType type)->uint32;
	auto ToShaderInOutString(EShaderInOutType type) -> const char*;
}

namespace vk {
	struct VertexAttribute : public df::VertexAttribute {
		auto MakeShaderSnippet(uint32 idx) const->df::String;
	};
}