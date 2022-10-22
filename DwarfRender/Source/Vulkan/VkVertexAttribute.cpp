#include "VkVertexAttribute.h"


auto df::ToShaderInOutType(df::EVertexAttributeFormat format)->df::EShaderInOutType {
	switch (format) {
	case df::EVertexAttributeFormat::X8_Norm:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY8_Norm:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ8_Norm:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW8_Norm:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_Norm:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_Norm:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_Norm:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_Norm:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X8_UNorm:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY8_UNorm:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ8_UNorm:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW8_UNorm:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_UNorm:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_UNorm:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_UNorm:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_UNorm:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_Float:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_Float:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_Float:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_Float:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X32_Float:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY32_Float:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ32_Float:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW32_Float:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X64_Float:
		return df::EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY64_Float:
		return df::EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ64_Float:
		return df::EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW64_Float:
		return df::EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X8_Int:
		return df::EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY8_Int:
		return df::EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ8_Int:
		return df::EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW8_Int:
		return df::EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X16_Int:
		return df::EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY16_Int:
		return df::EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ16_Int:
		return df::EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW16_Int:
		return df::EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X32_Int:
		return df::EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY32_Int:
		return df::EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ32_Int:
		return df::EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW32_Int:
		return df::EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X64_Int:
		return df::EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY64_Int:
		return df::EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ64_Int:
		return df::EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW64_Int:
		return df::EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X8_UInt:
		return df::EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY8_UInt:
		return df::EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ8_UInt:
		return df::EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW8_UInt:
		return df::EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X16_UInt:
		return df::EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY16_UInt:
		return df::EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ16_UInt:
		return df::EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW16_UInt:
		return df::EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X32_UInt:
		return df::EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY32_UInt:
		return df::EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ32_UInt:
		return df::EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW32_UInt:
		return df::EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X64_UInt:
		return df::EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY64_UInt:
		return df::EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ64_UInt:
		return df::EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW64_UInt:
		return df::EShaderInOutType::UIVec4;
	default:
		return df::EShaderInOutType::Float;
	}
}

auto df::ToAttributeSize(df::EVertexAttributeFormat format)->uint32 {
	return df::ToAttributeSize(ToShaderInOutType(format));
}

auto df::ToAttributeSize(df::EShaderInOutType type)->uint32 {
	switch (type) {
	case df::EShaderInOutType::Float:
		return sizeof(float) * 1;
	case df::EShaderInOutType::Vec2:
		return sizeof(float) * 2;
	case df::EShaderInOutType::Vec3:
		return sizeof(float) * 3;
	case df::EShaderInOutType::Vec4:
		return sizeof(float) * 4;
	case df::EShaderInOutType::Int:
		return sizeof(int32) * 1;
	case df::EShaderInOutType::IVec2:
		return sizeof(int32) * 2;
	case df::EShaderInOutType::IVec3:
		return sizeof(int32) * 3;
	case df::EShaderInOutType::IVec4:
		return sizeof(int32) * 4;
	case df::EShaderInOutType::UInt:
		return sizeof(uint32) * 1;
	case df::EShaderInOutType::UIVec2:
		return sizeof(uint32) * 2;
	case df::EShaderInOutType::UIVec3:
		return sizeof(uint32) * 3;
	case df::EShaderInOutType::UIVec4:
		return sizeof(uint32) * 4;
	default:
		return 0;
	}
}

auto df::ToShaderInOutString(df::EShaderInOutType type) -> const char* {
	switch (type) {
	case df::EShaderInOutType::Float:
		return "float";
	case df::EShaderInOutType::Vec2:
		return "vec2";
	case df::EShaderInOutType::Vec3:
		return "vec3";
	case df::EShaderInOutType::Vec4:
		return "vec4";
	case df::EShaderInOutType::Int:
		return "int";
	case df::EShaderInOutType::IVec2:
		return "ivec2";
	case df::EShaderInOutType::IVec3:
		return "ivec3";
	case df::EShaderInOutType::IVec4:
		return "ivec4";
	case df::EShaderInOutType::UInt:
		return "int";
	case df::EShaderInOutType::UIVec2:
		return "ivec2";
	case df::EShaderInOutType::UIVec3:
		return "ivec3";
	case df::EShaderInOutType::UIVec4:
		return "ivec4";
	default:
		return "";
	}
}