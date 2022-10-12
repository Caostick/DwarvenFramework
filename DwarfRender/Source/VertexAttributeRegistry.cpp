#include <DwarfRender/VertexAttributeRegistry.h>

uint32 rf::VertexAttributeRegistry::TypeCount = 0;
rf::VertexAttributeRegistry::Entry rf::VertexAttributeRegistry::Entries[MaxVertexAttributeTypes];

auto rf::VertexAttributeRegistry::FindByName(const df::StringView& name)->int32 {
	for (int32 i = 0; i < int32(TypeCount); ++i) {
		if (name == Entries[i].Name) {
			return i;
		}
	}
	return -1;
}

auto rf::ToShaderInOutType(rf::EVertexAttributeFormat format)->rf::EShaderInOutType {
	switch (format) {
	case rf::EVertexAttributeFormat::X8_Norm:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY8_Norm:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ8_Norm:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW8_Norm:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X16_Norm:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY16_Norm:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ16_Norm:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW16_Norm:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X8_UNorm:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY8_UNorm:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ8_UNorm:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW8_UNorm:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X16_UNorm:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY16_UNorm:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ16_UNorm:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW16_UNorm:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X16_Float:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY16_Float:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ16_Float:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW16_Float:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X32_Float:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY32_Float:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ32_Float:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW32_Float:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X64_Float:
		return rf::EShaderInOutType::Float;
	case rf::EVertexAttributeFormat::XY64_Float:
		return rf::EShaderInOutType::Vec2;
	case rf::EVertexAttributeFormat::XYZ64_Float:
		return rf::EShaderInOutType::Vec3;
	case rf::EVertexAttributeFormat::XYZW64_Float:
		return rf::EShaderInOutType::Vec4;
	case rf::EVertexAttributeFormat::X8_Int:
		return rf::EShaderInOutType::Int;
	case rf::EVertexAttributeFormat::XY8_Int:
		return rf::EShaderInOutType::IVec2;
	case rf::EVertexAttributeFormat::XYZ8_Int:
		return rf::EShaderInOutType::IVec3;
	case rf::EVertexAttributeFormat::XYZW8_Int:
		return rf::EShaderInOutType::IVec4;
	case rf::EVertexAttributeFormat::X16_Int:
		return rf::EShaderInOutType::Int;
	case rf::EVertexAttributeFormat::XY16_Int:
		return rf::EShaderInOutType::IVec2;
	case rf::EVertexAttributeFormat::XYZ16_Int:
		return rf::EShaderInOutType::IVec3;
	case rf::EVertexAttributeFormat::XYZW16_Int:
		return rf::EShaderInOutType::IVec4;
	case rf::EVertexAttributeFormat::X32_Int:
		return rf::EShaderInOutType::Int;
	case rf::EVertexAttributeFormat::XY32_Int:
		return rf::EShaderInOutType::IVec2;
	case rf::EVertexAttributeFormat::XYZ32_Int:
		return rf::EShaderInOutType::IVec3;
	case rf::EVertexAttributeFormat::XYZW32_Int:
		return rf::EShaderInOutType::IVec4;
	case rf::EVertexAttributeFormat::X64_Int:
		return rf::EShaderInOutType::Int;
	case rf::EVertexAttributeFormat::XY64_Int:
		return rf::EShaderInOutType::IVec2;
	case rf::EVertexAttributeFormat::XYZ64_Int:
		return rf::EShaderInOutType::IVec3;
	case rf::EVertexAttributeFormat::XYZW64_Int:
		return rf::EShaderInOutType::IVec4;
	case rf::EVertexAttributeFormat::X8_UInt:
		return rf::EShaderInOutType::UInt;
	case rf::EVertexAttributeFormat::XY8_UInt:
		return rf::EShaderInOutType::UIVec2;
	case rf::EVertexAttributeFormat::XYZ8_UInt:
		return rf::EShaderInOutType::UIVec3;
	case rf::EVertexAttributeFormat::XYZW8_UInt:
		return rf::EShaderInOutType::UIVec4;
	case rf::EVertexAttributeFormat::X16_UInt:
		return rf::EShaderInOutType::UInt;
	case rf::EVertexAttributeFormat::XY16_UInt:
		return rf::EShaderInOutType::UIVec2;
	case rf::EVertexAttributeFormat::XYZ16_UInt:
		return rf::EShaderInOutType::UIVec3;
	case rf::EVertexAttributeFormat::XYZW16_UInt:
		return rf::EShaderInOutType::UIVec4;
	case rf::EVertexAttributeFormat::X32_UInt:
		return rf::EShaderInOutType::UInt;
	case rf::EVertexAttributeFormat::XY32_UInt:
		return rf::EShaderInOutType::UIVec2;
	case rf::EVertexAttributeFormat::XYZ32_UInt:
		return rf::EShaderInOutType::UIVec3;
	case rf::EVertexAttributeFormat::XYZW32_UInt:
		return rf::EShaderInOutType::UIVec4;
	case rf::EVertexAttributeFormat::X64_UInt:
		return rf::EShaderInOutType::UInt;
	case rf::EVertexAttributeFormat::XY64_UInt:
		return rf::EShaderInOutType::UIVec2;
	case rf::EVertexAttributeFormat::XYZ64_UInt:
		return rf::EShaderInOutType::UIVec3;
	case rf::EVertexAttributeFormat::XYZW64_UInt:
		return rf::EShaderInOutType::UIVec4;
	default:
		return rf::EShaderInOutType::Float;
	}
}

auto rf::ToAttributeSize(rf::EVertexAttributeFormat format)->uint32 {
	return rf::ToAttributeSize(ToShaderInOutType(format));
}

auto rf::ToAttributeSize(rf::EShaderInOutType type)->uint32 {
	switch (type) {
	case rf::EShaderInOutType::Float:
		return sizeof(float) * 1;
	case rf::EShaderInOutType::Vec2:
		return sizeof(float) * 2;
	case rf::EShaderInOutType::Vec3:
		return sizeof(float) * 3;
	case rf::EShaderInOutType::Vec4:
		return sizeof(float) * 4;
	case rf::EShaderInOutType::Int:
		return sizeof(int32) * 1;
	case rf::EShaderInOutType::IVec2:
		return sizeof(int32) * 2;
	case rf::EShaderInOutType::IVec3:
		return sizeof(int32) * 3;
	case rf::EShaderInOutType::IVec4:
		return sizeof(int32) * 4;
	case rf::EShaderInOutType::UInt:
		return sizeof(uint32) * 1;
	case rf::EShaderInOutType::UIVec2:
		return sizeof(uint32) * 2;
	case rf::EShaderInOutType::UIVec3:
		return sizeof(uint32) * 3;
	case rf::EShaderInOutType::UIVec4:
		return sizeof(uint32) * 4;
	default:
		return 0;
	}
}

auto rf::ToShaderInOutString(rf::EShaderInOutType type) -> const char* {
	switch (type) {
	case rf::EShaderInOutType::Float:
		return "float";
	case rf::EShaderInOutType::Vec2:
		return "vec2";
	case rf::EShaderInOutType::Vec3:
		return "vec3";
	case rf::EShaderInOutType::Vec4:
		return "vec4";
	case rf::EShaderInOutType::Int:
		return "int";
	case rf::EShaderInOutType::IVec2:
		return "ivec2";
	case rf::EShaderInOutType::IVec3:
		return "ivec3";
	case rf::EShaderInOutType::IVec4:
		return "ivec4";
	case rf::EShaderInOutType::UInt:
		return "int";
	case rf::EShaderInOutType::UIVec2:
		return "ivec2";
	case rf::EShaderInOutType::UIVec3:
		return "ivec3";
	case rf::EShaderInOutType::UIVec4:
		return "ivec4";
	default:
		return "";
	}
}