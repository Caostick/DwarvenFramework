#include "VkVertexAttribute.h"


auto vk::ToShaderInOutType(df::EVertexAttributeFormat format)->EShaderInOutType {
	switch (format) {
	case df::EVertexAttributeFormat::X8_Norm:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY8_Norm:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ8_Norm:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW8_Norm:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_Norm:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_Norm:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_Norm:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_Norm:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X8_UNorm:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY8_UNorm:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ8_UNorm:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW8_UNorm:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_UNorm:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_UNorm:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_UNorm:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_UNorm:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X16_Float:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY16_Float:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ16_Float:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW16_Float:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X32_Float:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY32_Float:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ32_Float:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW32_Float:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X64_Float:
		return EShaderInOutType::Float;
	case df::EVertexAttributeFormat::XY64_Float:
		return EShaderInOutType::Vec2;
	case df::EVertexAttributeFormat::XYZ64_Float:
		return EShaderInOutType::Vec3;
	case df::EVertexAttributeFormat::XYZW64_Float:
		return EShaderInOutType::Vec4;
	case df::EVertexAttributeFormat::X8_Int:
		return EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY8_Int:
		return EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ8_Int:
		return EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW8_Int:
		return EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X16_Int:
		return EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY16_Int:
		return EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ16_Int:
		return EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW16_Int:
		return EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X32_Int:
		return EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY32_Int:
		return EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ32_Int:
		return EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW32_Int:
		return EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X64_Int:
		return EShaderInOutType::Int;
	case df::EVertexAttributeFormat::XY64_Int:
		return EShaderInOutType::IVec2;
	case df::EVertexAttributeFormat::XYZ64_Int:
		return EShaderInOutType::IVec3;
	case df::EVertexAttributeFormat::XYZW64_Int:
		return EShaderInOutType::IVec4;
	case df::EVertexAttributeFormat::X8_UInt:
		return EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY8_UInt:
		return EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ8_UInt:
		return EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW8_UInt:
		return EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X16_UInt:
		return EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY16_UInt:
		return EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ16_UInt:
		return EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW16_UInt:
		return EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X32_UInt:
		return EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY32_UInt:
		return EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ32_UInt:
		return EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW32_UInt:
		return EShaderInOutType::UIVec4;
	case df::EVertexAttributeFormat::X64_UInt:
		return EShaderInOutType::UInt;
	case df::EVertexAttributeFormat::XY64_UInt:
		return EShaderInOutType::UIVec2;
	case df::EVertexAttributeFormat::XYZ64_UInt:
		return EShaderInOutType::UIVec3;
	case df::EVertexAttributeFormat::XYZW64_UInt:
		return EShaderInOutType::UIVec4;
	default:
		return EShaderInOutType::Float;
	}
}

auto vk::ToDataStride(df::EVertexAttributeFormat format) -> uint32 {
	switch (format) {
	case df::EVertexAttributeFormat::X8_Norm:
		return 1;
	case df::EVertexAttributeFormat::XY8_Norm:
		return 2;
	case df::EVertexAttributeFormat::XYZ8_Norm:
		return 3;
	case df::EVertexAttributeFormat::XYZW8_Norm:
		return 4;
	case df::EVertexAttributeFormat::X16_Norm:
		return 2;
	case df::EVertexAttributeFormat::XY16_Norm:
		return 4;
	case df::EVertexAttributeFormat::XYZ16_Norm:
		return 6;
	case df::EVertexAttributeFormat::XYZW16_Norm:
		return 8;
	case df::EVertexAttributeFormat::X8_UNorm:
		return 1;
	case df::EVertexAttributeFormat::XY8_UNorm:
		return 2;
	case df::EVertexAttributeFormat::XYZ8_UNorm:
		return 3;
	case df::EVertexAttributeFormat::XYZW8_UNorm:
		return 4;
	case df::EVertexAttributeFormat::X16_UNorm:
		return 2;
	case df::EVertexAttributeFormat::XY16_UNorm:
		return 4;
	case df::EVertexAttributeFormat::XYZ16_UNorm:
		return 6;
	case df::EVertexAttributeFormat::XYZW16_UNorm:
		return 6;
	case df::EVertexAttributeFormat::X16_Float:
		return 2;
	case df::EVertexAttributeFormat::XY16_Float:
		return 4;
	case df::EVertexAttributeFormat::XYZ16_Float:
		return 6;
	case df::EVertexAttributeFormat::XYZW16_Float:
		return 8;
	case df::EVertexAttributeFormat::X32_Float:
		return 4;
	case df::EVertexAttributeFormat::XY32_Float:
		return 8;
	case df::EVertexAttributeFormat::XYZ32_Float:
		return 12;
	case df::EVertexAttributeFormat::XYZW32_Float:
		return 16;
	case df::EVertexAttributeFormat::X64_Float:
		return 8;
	case df::EVertexAttributeFormat::XY64_Float:
		return 16;
	case df::EVertexAttributeFormat::XYZ64_Float:
		return 24;
	case df::EVertexAttributeFormat::XYZW64_Float:
		return 32;
	case df::EVertexAttributeFormat::X8_Int:
		return 1;
	case df::EVertexAttributeFormat::XY8_Int:
		return 2;
	case df::EVertexAttributeFormat::XYZ8_Int:
		return 3;
	case df::EVertexAttributeFormat::XYZW8_Int:
		return 4;
	case df::EVertexAttributeFormat::X16_Int:
		return 2;
	case df::EVertexAttributeFormat::XY16_Int:
		return 4;
	case df::EVertexAttributeFormat::XYZ16_Int:
		return 6;
	case df::EVertexAttributeFormat::XYZW16_Int:
		return 8;
	case df::EVertexAttributeFormat::X32_Int:
		return 4;
	case df::EVertexAttributeFormat::XY32_Int:
		return 8;
	case df::EVertexAttributeFormat::XYZ32_Int:
		return 12;
	case df::EVertexAttributeFormat::XYZW32_Int:
		return 16;
	case df::EVertexAttributeFormat::X64_Int:
		return 8;
	case df::EVertexAttributeFormat::XY64_Int:
		return 16;
	case df::EVertexAttributeFormat::XYZ64_Int:
		return 24;
	case df::EVertexAttributeFormat::XYZW64_Int:
		return 32;
	case df::EVertexAttributeFormat::X8_UInt:
		return 1;
	case df::EVertexAttributeFormat::XY8_UInt:
		return 2;
	case df::EVertexAttributeFormat::XYZ8_UInt:
		return 3;
	case df::EVertexAttributeFormat::XYZW8_UInt:
		return 4;
	case df::EVertexAttributeFormat::X16_UInt:
		return 2;
	case df::EVertexAttributeFormat::XY16_UInt:
		return 4;
	case df::EVertexAttributeFormat::XYZ16_UInt:
		return 6;
	case df::EVertexAttributeFormat::XYZW16_UInt:
		return 8;
	case df::EVertexAttributeFormat::X32_UInt:
		return 4;
	case df::EVertexAttributeFormat::XY32_UInt:
		return 8;
	case df::EVertexAttributeFormat::XYZ32_UInt:
		return 12;
	case df::EVertexAttributeFormat::XYZW32_UInt:
		return 16;
	case df::EVertexAttributeFormat::X64_UInt:
		return 8;
	case df::EVertexAttributeFormat::XY64_UInt:
		return 16;
	case df::EVertexAttributeFormat::XYZ64_UInt:
		return 24;
	case df::EVertexAttributeFormat::XYZW64_UInt:
		return 32;
	default:
		return 0;
	}
}

auto vk::ToVkFormat(df::EVertexAttributeFormat format)->VkFormat {
	switch (format) {
	case df::EVertexAttributeFormat::X8_Norm:
		return VK_FORMAT_R8_SNORM;
	case df::EVertexAttributeFormat::XY8_Norm:
		return VK_FORMAT_R8G8_SNORM;
	case df::EVertexAttributeFormat::XYZ8_Norm:
		return VK_FORMAT_R8G8B8_SNORM;
	case df::EVertexAttributeFormat::XYZW8_Norm:
		return VK_FORMAT_R8G8B8A8_SNORM;
	case df::EVertexAttributeFormat::X16_Norm:
		return VK_FORMAT_R16_SNORM;
	case df::EVertexAttributeFormat::XY16_Norm:
		return VK_FORMAT_R16G16_SNORM;
	case df::EVertexAttributeFormat::XYZ16_Norm:
		return VK_FORMAT_R16G16B16_SNORM;
	case df::EVertexAttributeFormat::XYZW16_Norm:
		return VK_FORMAT_R16G16B16A16_SNORM;

	case df::EVertexAttributeFormat::X8_UNorm:
		return VK_FORMAT_R8_UNORM;
	case df::EVertexAttributeFormat::XY8_UNorm:
		return VK_FORMAT_R8G8_UNORM;
	case df::EVertexAttributeFormat::XYZ8_UNorm:
		return VK_FORMAT_R8G8B8_UNORM;
	case df::EVertexAttributeFormat::XYZW8_UNorm:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case df::EVertexAttributeFormat::X16_UNorm:
		return VK_FORMAT_R16_UNORM;
	case df::EVertexAttributeFormat::XY16_UNorm:
		return VK_FORMAT_R16G16_UNORM;
	case df::EVertexAttributeFormat::XYZ16_UNorm:
		return VK_FORMAT_R16G16B16_UNORM;
	case df::EVertexAttributeFormat::XYZW16_UNorm:
		return VK_FORMAT_R16G16B16A16_UNORM;

	case df::EVertexAttributeFormat::X8_UInt:
		return VK_FORMAT_R8_UINT;
	case df::EVertexAttributeFormat::X8_Int:
		return VK_FORMAT_R8_SINT;
	case df::EVertexAttributeFormat::XY8_UInt:
		return VK_FORMAT_R8G8_UINT;
	case df::EVertexAttributeFormat::XY8_Int:
		return VK_FORMAT_R8G8_SINT;
	case df::EVertexAttributeFormat::XYZ8_UInt:
		return VK_FORMAT_R8G8B8_UINT;
	case df::EVertexAttributeFormat::XYZ8_Int:
		return VK_FORMAT_R8G8B8_SINT;
	case df::EVertexAttributeFormat::XYZW8_UInt:
		return VK_FORMAT_R8G8B8A8_UINT;
	case df::EVertexAttributeFormat::XYZW8_Int:
		return VK_FORMAT_R8G8B8A8_SINT;

	case df::EVertexAttributeFormat::X16_Float:
		return VK_FORMAT_R16_SFLOAT;
	case df::EVertexAttributeFormat::XY16_Float:
		return VK_FORMAT_R16G16_SFLOAT;
	case df::EVertexAttributeFormat::XYZ16_Float:
		return VK_FORMAT_R16G16B16_SFLOAT;
	case df::EVertexAttributeFormat::XYZW16_Float:
		return VK_FORMAT_R16G16B16A16_SFLOAT;
	case df::EVertexAttributeFormat::X32_Float:
		return VK_FORMAT_R32_SFLOAT;
	case df::EVertexAttributeFormat::XY32_Float:
		return VK_FORMAT_R32G32_SFLOAT;
	case df::EVertexAttributeFormat::XYZ32_Float:
		return VK_FORMAT_R32G32B32_SFLOAT;
	case df::EVertexAttributeFormat::XYZW32_Float:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case df::EVertexAttributeFormat::X64_Float:
		return VK_FORMAT_R64_SFLOAT;
	case df::EVertexAttributeFormat::XY64_Float:
		return VK_FORMAT_R64G64_SFLOAT;
	case df::EVertexAttributeFormat::XYZ64_Float:
		return VK_FORMAT_R64G64B64_SFLOAT;
	case df::EVertexAttributeFormat::XYZW64_Float:
		return VK_FORMAT_R64G64B64A64_SFLOAT;

	case df::EVertexAttributeFormat::X16_Int:
		return VK_FORMAT_R16_SINT;
	case df::EVertexAttributeFormat::XY16_Int:
		return VK_FORMAT_R16G16_SINT;
	case df::EVertexAttributeFormat::XYZ16_Int:
		return VK_FORMAT_R16G16B16_SINT;
	case df::EVertexAttributeFormat::XYZW16_Int:
		return VK_FORMAT_R16G16B16A16_SINT;
	case df::EVertexAttributeFormat::X32_Int:
		return VK_FORMAT_R32_SINT;
	case df::EVertexAttributeFormat::XY32_Int:
		return VK_FORMAT_R32G32_SINT;
	case df::EVertexAttributeFormat::XYZ32_Int:
		return VK_FORMAT_R32G32B32_SINT;
	case df::EVertexAttributeFormat::XYZW32_Int:
		return VK_FORMAT_R32G32B32A32_SINT;
	case df::EVertexAttributeFormat::X64_Int:
		return VK_FORMAT_R64_SINT;
	case df::EVertexAttributeFormat::XY64_Int:
		return VK_FORMAT_R64G64_SINT;
	case df::EVertexAttributeFormat::XYZ64_Int:
		return VK_FORMAT_R64G64B64_SINT;
	case df::EVertexAttributeFormat::XYZW64_Int:
		return VK_FORMAT_R64G64B64A64_SINT;

	case df::EVertexAttributeFormat::X16_UInt:
		return VK_FORMAT_R16_UINT;
	case df::EVertexAttributeFormat::XY16_UInt:
		return VK_FORMAT_R16G16_UINT;
	case df::EVertexAttributeFormat::XYZ16_UInt:
		return VK_FORMAT_R16G16B16_UINT;
	case df::EVertexAttributeFormat::XYZW16_UInt:
		return VK_FORMAT_R16G16B16A16_UINT;
	case df::EVertexAttributeFormat::X32_UInt:
		return VK_FORMAT_R32_UINT;
	case df::EVertexAttributeFormat::XY32_UInt:
		return VK_FORMAT_R32G32_UINT;
	case df::EVertexAttributeFormat::XYZ32_UInt:
		return VK_FORMAT_R32G32B32_UINT;
	case df::EVertexAttributeFormat::XYZW32_UInt:
		return VK_FORMAT_R32G32B32A32_UINT;
	case df::EVertexAttributeFormat::X64_UInt:
		return VK_FORMAT_R64_UINT;
	case df::EVertexAttributeFormat::XY64_UInt:
		return VK_FORMAT_R64G64_UINT;
	case df::EVertexAttributeFormat::XYZ64_UInt:
		return VK_FORMAT_R64G64B64_UINT;
	case df::EVertexAttributeFormat::XYZW64_UInt:
		return VK_FORMAT_R64G64B64A64_UINT;

	default:
		return VK_FORMAT_UNDEFINED;
	}
}

auto vk::ToAttributeSize(df::EVertexAttributeFormat format)->uint32 {
	return ToAttributeSize(ToShaderInOutType(format));
}

auto vk::ToAttributeSize(EShaderInOutType type)->uint32 {
	switch (type) {
	case EShaderInOutType::Float:
		return sizeof(float) * 1;
	case EShaderInOutType::Vec2:
		return sizeof(float) * 2;
	case EShaderInOutType::Vec3:
		return sizeof(float) * 3;
	case EShaderInOutType::Vec4:
		return sizeof(float) * 4;
	case EShaderInOutType::Int:
		return sizeof(int32) * 1;
	case EShaderInOutType::IVec2:
		return sizeof(int32) * 2;
	case EShaderInOutType::IVec3:
		return sizeof(int32) * 3;
	case EShaderInOutType::IVec4:
		return sizeof(int32) * 4;
	case EShaderInOutType::UInt:
		return sizeof(uint32) * 1;
	case EShaderInOutType::UIVec2:
		return sizeof(uint32) * 2;
	case EShaderInOutType::UIVec3:
		return sizeof(uint32) * 3;
	case EShaderInOutType::UIVec4:
		return sizeof(uint32) * 4;
	default:
		return 0;
	}
}

auto vk::ToShaderInOutString(EShaderInOutType type) -> const char* {
	switch (type) {
	case EShaderInOutType::Float:
		return "float";
	case EShaderInOutType::Vec2:
		return "vec2";
	case EShaderInOutType::Vec3:
		return "vec3";
	case EShaderInOutType::Vec4:
		return "vec4";
	case EShaderInOutType::Int:
		return "int";
	case EShaderInOutType::IVec2:
		return "ivec2";
	case EShaderInOutType::IVec3:
		return "ivec3";
	case EShaderInOutType::IVec4:
		return "ivec4";
	case EShaderInOutType::UInt:
		return "int";
	case EShaderInOutType::UIVec2:
		return "ivec2";
	case EShaderInOutType::UIVec3:
		return "ivec3";
	case EShaderInOutType::UIVec4:
		return "ivec4";
	default:
		return "";
	}
}

auto vk::VertexAttribute::MakeShaderSnippet(uint32 idx) const->df::String {
	const auto strLocation = std::to_string(idx);
	const auto strType = ToShaderInOutString(ToShaderInOutType(m_Format));
	const auto& strName = m_Name;

	return df::String("layout(location = ") + strLocation + ") in " + strType + df::String(" ") + strName + df::String(";\n");
}

auto vk::VertexAttribute::MakeShaderSnippet() const->df::String {
	const auto strType = ToShaderInOutString(ToShaderInOutType(m_Format));
	const auto& strName = m_Name;

	return df::String("layout(location = #) in ") + strType + df::String(" ") + strName + df::String(";\n");
}