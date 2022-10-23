#pragma once

#include "VkAPI.h"

#include <DwarfRender/VertexAttribute.h>

namespace df {
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

	auto ToShaderInOutType(df::EVertexAttributeFormat format)->df::EShaderInOutType;
	auto ToStride(df::EVertexAttributeFormat format)->uint32;
	auto ToVkFormat(df::EVertexAttributeFormat format)->VkFormat;
	auto ToAttributeSize(df::EVertexAttributeFormat format)->uint32;
	auto ToAttributeSize(df::EShaderInOutType type)->uint32;
	auto ToShaderInOutString(df::EShaderInOutType type) -> const char*;
}

namespace vk {
	struct VertexAttribute : public df::VertexAttribute {
		df::String m_ShaderString;
	};
}