#pragma once

#include <DwarvenCore/EnumFlags.h>

namespace rf {
	enum class EFormat {
		Undefined,
		R8,
		R8G8,
		R8G8B8,
		R8G8B8A8,
		R16,
		R16G16,
		R16G16B16,
		R16G16B16A16,
		R32I,
		R32UI,
		R32F,
		R32G32F,
		R32G16B32F,
		R32G32B32A32F,
		D32,
		D24S8,

		COUNT
	};

	enum class EBufferFormat {
		Undefined,

		COUNT
	};

	enum class ETextureFormat {
		Undefined,

		D32_Float,
		D24_Float_S8_UInt,

		R8_UNorm,
		R16_UNorm,
		R8G8_UNorm,
		R8G8B8A8_UNorm,
		A2R10G10B10_UNorm,

		R16_Float,
		R32_Float,
		R16G16_Float,
		R16G16B16A16_Float,

		COUNT
	};

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

	enum class EVertexAttributeFormat {
		X8_Norm,
		XY8_Norm,
		XYZ8_Norm,
		XYZW8_Norm,
		X16_Norm,
		XY16_Norm,
		XYZ16_Norm,
		XYZW16_Norm,

		X8_UNorm,
		XY8_UNorm,
		XYZ8_UNorm,
		XYZW8_UNorm,
		X16_UNorm,
		XY16_UNorm,
		XYZ16_UNorm,
		XYZW16_UNorm,

		X16_Float,
		XY16_Float,
		XYZ16_Float,
		XYZW16_Float,
		X32_Float,
		XY32_Float,
		XYZ32_Float,
		XYZW32_Float,
		X64_Float,
		XY64_Float,
		XYZ64_Float,
		XYZW64_Float,

		X8_Int,
		XY8_Int,
		XYZ8_Int,
		XYZW8_Int,
		X16_Int,
		XY16_Int,
		XYZ16_Int,
		XYZW16_Int,
		X32_Int,
		XY32_Int,
		XYZ32_Int,
		XYZW32_Int,
		X64_Int,
		XY64_Int,
		XYZ64_Int,
		XYZW64_Int,

		X8_UInt,
		XY8_UInt,
		XYZ8_UInt,
		XYZW8_UInt,
		X16_UInt,
		XY16_UInt,
		XYZ16_UInt,
		XYZW16_UInt,
		X32_UInt,
		XY32_UInt,
		XYZ32_UInt,
		XYZW32_UInt,
		X64_UInt,
		XY64_UInt,
		XYZ64_UInt,
		XYZW64_UInt,

		COUNT
	};

	enum class EShaderParameterType {
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4
	};

	enum class EDescriptorType {
		None,
		SamplerState,
		Image,
		CombinedImageSampler,
		UniformBuffer,
		UniformTexelBuffer,
		DynamicUniformBuffer,
		StorageBuffer,
		StorageTexelBuffer,
		DynamicStorageBuffer,

		COUNT
	};

	enum class EBufferUsageFlag {
		Storage = 1 << 0,
		Uniform = 1 << 1,
		StorageTexel = 1 << 2,
		UniformTexel = 1 << 3,
		Vertex = 1 << 4,
		Index = 1 << 5,
		Indirect = 1 << 6
	};

	enum class EImageUsageFlag {
		Texture = 1 << 0,
		RenderTarget = 1 << 1,
		DepthRenderTarget = 1 << 2,
	};

	enum class EBufferAccessType {
		Transfer,
		Direct
	};

	enum class EImageLayout {
		Undefined,
		General,
		ColorAttachment,
		ColorReadOnly,
		DepthStencilAttachment,
		DepthStencilReadOnly,
		DepthReadOnlyStencilAttachment,
		DepthAttachmentStencilReadOnly,
		DepthAttachment,
		DepthReadOnly,
		StencilAttachment,
		StencilReadOnly,
		TransferSrc,
		TransferDst,

		COUNT
	};

	enum class EBufferLayout {
		Undefined,
		General,
		ColorAttachment,
		ColorReadOnly,
		DepthStencilAttachment,
		DepthStencilReadOnly,
		DepthReadOnlyStencilAttachment,
		DepthAttachmentStencilReadOnly,
		DepthAttachment,
		DepthReadOnly,
		StencilAttachment,
		StencilReadOnly,
		TransferSrc,
		TransferDst,

		COUNT
	};

	using BufferUsageFlags = df::EnumFlags<EBufferUsageFlag>;
	DF_DEFINE_ENUM_FLAG_OPERATORS(BufferUsageFlags);

	using ImageUsageFlags = df::EnumFlags<EImageUsageFlag>;
	DF_DEFINE_ENUM_FLAG_OPERATORS(ImageUsageFlags);
}