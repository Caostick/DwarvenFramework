#include "VkTypeConversions.h"

auto rf::api::ToVk(rf::EPolygonMode value)->VkPolygonMode {
	switch (value) {
	case rf::EPolygonMode::Fill:
		return VK_POLYGON_MODE_FILL;
	case rf::EPolygonMode::Line:
		return VK_POLYGON_MODE_LINE;
	case rf::EPolygonMode::Point:
		return VK_POLYGON_MODE_POINT;
	default:
		return VK_POLYGON_MODE_FILL;
	}
}

auto rf::api::ToVk(rf::ECullMode value)->VkCullModeFlagBits {
	switch (value) {
	case rf::ECullMode::None:
		return VK_CULL_MODE_NONE;
	case rf::ECullMode::Front:
		return VK_CULL_MODE_FRONT_BIT;
	case rf::ECullMode::Back:
		return VK_CULL_MODE_BACK_BIT;
	case rf::ECullMode::FrontAndBack:
		return VK_CULL_MODE_FRONT_AND_BACK;
	default:
		return VK_CULL_MODE_NONE;
	}
}

auto rf::api::ToVk(rf::EFrontFace value)->VkFrontFace {
	switch (value) {
	case rf::EFrontFace::CounterClockwise:
		return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	case rf::EFrontFace::Clockwise:
		return VK_FRONT_FACE_CLOCKWISE;
	default:
		return VK_FRONT_FACE_CLOCKWISE;
	}
}

auto rf::api::ToVk(rf::EBlendFactor value)->VkBlendFactor {
	switch (value) {
	case rf::EBlendFactor::Zero:
		return VK_BLEND_FACTOR_ZERO;
	case rf::EBlendFactor::One:
		return VK_BLEND_FACTOR_ONE;
	case rf::EBlendFactor::SrcColor:
		return VK_BLEND_FACTOR_SRC_COLOR;
	case rf::EBlendFactor::OneMinusSrcColor:
		return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
	case rf::EBlendFactor::DstColor:
		return VK_BLEND_FACTOR_DST_COLOR;
	case rf::EBlendFactor::OneMinusDstColor:
		return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
	case rf::EBlendFactor::SrcAlpha:
		return VK_BLEND_FACTOR_SRC_ALPHA;
	case rf::EBlendFactor::OneMinusSrcAlpha:
		return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	case rf::EBlendFactor::DstAlpha:
		return VK_BLEND_FACTOR_DST_ALPHA;
	case rf::EBlendFactor::OneMinusDstAlpha:
		return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
	default:
		return VK_BLEND_FACTOR_ZERO;
	}
}

auto rf::api::ToVk(rf::EBlendOp value)->VkBlendOp {
	switch (value) {
	case rf::EBlendOp::Add:
		return VK_BLEND_OP_ADD;
	case rf::EBlendOp::Subtract:
		return VK_BLEND_OP_SUBTRACT;
	case rf::EBlendOp::ReverseSubtract:
		return VK_BLEND_OP_REVERSE_SUBTRACT;
	case rf::EBlendOp::Min:
		return VK_BLEND_OP_MIN;
	case rf::EBlendOp::Max:
		return VK_BLEND_OP_MAX;
	default:
		return VK_BLEND_OP_ADD;
	}
}

auto rf::api::ToVk(rf::EDepthCompareOp value)->VkCompareOp {
	switch (value) {
	case rf::EDepthCompareOp::Never:
		return VK_COMPARE_OP_NEVER;
	case rf::EDepthCompareOp::Equal:
		return VK_COMPARE_OP_EQUAL;
	case rf::EDepthCompareOp::NotEqual:
		return VK_COMPARE_OP_NOT_EQUAL;
	case rf::EDepthCompareOp::Greater:
		return VK_COMPARE_OP_GREATER;
	case rf::EDepthCompareOp::GreaterEqual:
		return VK_COMPARE_OP_GREATER_OR_EQUAL;
	case rf::EDepthCompareOp::Less:
		return VK_COMPARE_OP_LESS;
	case rf::EDepthCompareOp::LessEqual:
		return VK_COMPARE_OP_LESS_OR_EQUAL;
	case rf::EDepthCompareOp::Always:
		return VK_COMPARE_OP_ALWAYS;
	default:
		return VK_COMPARE_OP_EQUAL;
	}
}

auto rf::api::ToVk(rf::EStencilOp value)->VkStencilOp {
	switch (value) {
	case rf::EStencilOp::Keep:
		return VK_STENCIL_OP_KEEP;
	case rf::EStencilOp::Zero:
		return VK_STENCIL_OP_ZERO;
	case rf::EStencilOp::Replace:
		return VK_STENCIL_OP_REPLACE;
	case rf::EStencilOp::Increment:
		return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
	case rf::EStencilOp::Decrement:
		return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
	case rf::EStencilOp::Invert:
		return VK_STENCIL_OP_INVERT;
	default:
		return VK_STENCIL_OP_KEEP;
	}
}

auto rf::api::ToVk(rf::EAttachmentLoadOp value)->VkAttachmentLoadOp {
	switch (value) {
	case rf::EAttachmentLoadOp::DontCare:
		return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	case rf::EAttachmentLoadOp::Clear:
		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	case rf::EAttachmentLoadOp::Load:
		return VK_ATTACHMENT_LOAD_OP_LOAD;
	default:
		return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}
}

auto rf::api::ToVk(rf::EAttachmentStoreOp value)->VkAttachmentStoreOp {
	switch (value) {
	case rf::EAttachmentStoreOp::DontCare:
		return VK_ATTACHMENT_STORE_OP_DONT_CARE;
	case rf::EAttachmentStoreOp::Store:
		return VK_ATTACHMENT_STORE_OP_STORE;
	default:
		return VK_ATTACHMENT_STORE_OP_DONT_CARE;
	}
}

auto rf::api::ToVk(rf::EImageLayout value)->VkImageLayout {
	switch (value) {
	case rf::EImageLayout::Undefined:
		return VK_IMAGE_LAYOUT_UNDEFINED;
	case rf::EImageLayout::General:
		return VK_IMAGE_LAYOUT_GENERAL;
	case rf::EImageLayout::ColorAttachment:
		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	case rf::EImageLayout::ColorReadOnly:
		return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	case rf::EImageLayout::DepthStencilAttachment:
		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	case rf::EImageLayout::DepthStencilReadOnly:
		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
	case rf::EImageLayout::DepthReadOnlyStencilAttachment:
		return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
	case rf::EImageLayout::DepthAttachmentStencilReadOnly:
		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
	case rf::EImageLayout::DepthAttachment:
		return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
	case rf::EImageLayout::DepthReadOnly:
		return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
	case rf::EImageLayout::StencilAttachment:
		return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
	case rf::EImageLayout::StencilReadOnly:
		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
	case rf::EImageLayout::TransferSrc:
		return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	case rf::EImageLayout::TransferDst:
		return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	default:
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

auto rf::api::ToVk(rf::EFormat value)->VkFormat {
	switch (value) {
	case rf::EFormat::Undefined:
		return VK_FORMAT_UNDEFINED;
	case rf::EFormat::R8:
		return VK_FORMAT_R8_UNORM;
	case rf::EFormat::R8G8:
		return VK_FORMAT_R8G8_UNORM;
	case rf::EFormat::R8G8B8:
		return VK_FORMAT_R8G8B8_UNORM;
	case rf::EFormat::R8G8B8A8:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case rf::EFormat::R16:
		return VK_FORMAT_R16_SFLOAT;
	case rf::EFormat::R16G16:
		return VK_FORMAT_R16G16_SFLOAT;
	case rf::EFormat::R16G16B16:
		return VK_FORMAT_R16G16B16_SFLOAT;
	case rf::EFormat::R16G16B16A16:
		return VK_FORMAT_R16G16B16A16_SFLOAT;
	case rf::EFormat::R32I:
		return VK_FORMAT_R32_SINT;
	case rf::EFormat::R32UI:
		return VK_FORMAT_R32_UINT;
	case rf::EFormat::R32F:
		return VK_FORMAT_R32_SFLOAT;
	case rf::EFormat::R32G32F:
		return VK_FORMAT_R32G32_SFLOAT;
	case rf::EFormat::R32G16B32F:
		return VK_FORMAT_R32G32B32_SFLOAT;
	case rf::EFormat::R32G32B32A32F:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case rf::EFormat::D32:
		return VK_FORMAT_D32_SFLOAT;
	case rf::EFormat::D24S8:
		return VK_FORMAT_D24_UNORM_S8_UINT;
	default:
		return VK_FORMAT_UNDEFINED;
	}
}

auto rf::api::ToVk(rf::EDescriptorType value)->VkDescriptorType {
	switch (value) {
	case rf::EDescriptorType::SamplerState:
		return VK_DESCRIPTOR_TYPE_SAMPLER;
	case rf::EDescriptorType::Image:
		return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	case rf::EDescriptorType::CombinedImageSampler:
		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	case rf::EDescriptorType::UniformBuffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	case rf::EDescriptorType::UniformTexelBuffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	case rf::EDescriptorType::DynamicUniformBuffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	case rf::EDescriptorType::StorageBuffer:
		return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	case rf::EDescriptorType::StorageTexelBuffer:
		return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	case rf::EDescriptorType::DynamicStorageBuffer:
		return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	default:
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}
}

auto rf::api::ToVk(rf::EFilter value)->VkFilter {
	switch (value) {
	case rf::EFilter::Nearest:
		return VK_FILTER_NEAREST;
	case rf::EFilter::Linear:
		return VK_FILTER_LINEAR;
	default:
		return VK_FILTER_MAX_ENUM;
	}
}

auto rf::api::ToVk(rf::EMipmapMode value)->VkSamplerMipmapMode {
	switch (value) {
	case rf::EMipmapMode::Nearest:
		return VK_SAMPLER_MIPMAP_MODE_NEAREST;
	case rf::EMipmapMode::Linear:
		return VK_SAMPLER_MIPMAP_MODE_LINEAR;
	default:
		return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
	}
}

auto rf::api::ToVk(rf::EAddressMode value)->VkSamplerAddressMode {
	switch (value) {
	case rf::EAddressMode::Repeat:
		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	case rf::EAddressMode::MirroredRepeat:
		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	case rf::EAddressMode::ClampToEdge:
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	case rf::EAddressMode::ClampToBorder:
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	case rf::EAddressMode::MirrorClampToEdge:
		return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR;
	default:
		return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
	}
}

auto rf::api::ToVk(rf::ShaderStageFlags flags)->VkShaderStageFlags {
	VkShaderStageFlags result = 0;


	if (flags.Has(rf::EShaderStage::Vertex)) {
		result |= VK_SHADER_STAGE_VERTEX_BIT;
	}

	if (flags.Has(rf::EShaderStage::TesselationControl)) {
		result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	}

	if (flags.Has(rf::EShaderStage::TesselationEvaluation)) {
		result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	}

	if (flags.Has(rf::EShaderStage::Geometry)) {
		result |= VK_SHADER_STAGE_GEOMETRY_BIT;
	}

	if (flags.Has(rf::EShaderStage::Fragment)) {
		result |= VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	if (flags.Has(rf::EShaderStage::Compute)) {
		result |= VK_SHADER_STAGE_COMPUTE_BIT;
	}

	if (flags.Has(rf::EShaderStage::Raygen)) {
		result |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
	}

	if (flags.Has(rf::EShaderStage::AnyHit)) {
		result |= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
	}

	if (flags.Has(rf::EShaderStage::ClosestHit)) {
		result |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
	}

	if (flags.Has(rf::EShaderStage::Miss)) {
		result |= VK_SHADER_STAGE_MISS_BIT_KHR;
	}

	if (flags.Has(rf::EShaderStage::Intersection)) {
		result |= VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
	}

	return result;
}

auto rf::api::ToVk(rf::BufferUsageFlags flags)->VkBufferUsageFlags {
	VkBufferUsageFlags bufferUsageFlags = 0;

	if (flags.Has(rf::EBufferUsageFlag::Index)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::Storage)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::Uniform)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::StorageTexel)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::UniformTexel)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::Vertex)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	}

	if (flags.Has(rf::EBufferUsageFlag::Indirect)) {
		bufferUsageFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
	}

	return bufferUsageFlags;
}

auto rf::api::ToVk(rf::ImageUsageFlags flags)->VkImageUsageFlags {
	VkImageUsageFlags imageUsageFlags = 0;

	if (flags.Has(rf::EImageUsageFlag::Texture)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	if (flags.Has(rf::EImageUsageFlag::RenderTarget)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}

	if (flags.Has(rf::EImageUsageFlag::DepthRenderTarget)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}

	return imageUsageFlags;
}

auto rf::api::ToVk(rf::EPrimitiveTopology topology)->VkPrimitiveTopology {
	switch (topology) {
	case rf::EPrimitiveTopology::Points:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case rf::EPrimitiveTopology::Lines:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case rf::EPrimitiveTopology::Triangles:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	default:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
}

auto rf::api::ToVk(rf::EVertexAttributeFormat format)->VkFormat {
	switch (format) {
	case rf::EVertexAttributeFormat::X8_Norm:
		return VK_FORMAT_R8_SNORM;
	case rf::EVertexAttributeFormat::XY8_Norm:
		return VK_FORMAT_R8G8_SNORM;
	case rf::EVertexAttributeFormat::XYZ8_Norm:
		return VK_FORMAT_R8G8B8_SNORM;
	case rf::EVertexAttributeFormat::XYZW8_Norm:
		return VK_FORMAT_R8G8B8A8_SNORM;
	case rf::EVertexAttributeFormat::X16_Norm:
		return VK_FORMAT_R16_SNORM;
	case rf::EVertexAttributeFormat::XY16_Norm:
		return VK_FORMAT_R16G16_SNORM;
	case rf::EVertexAttributeFormat::XYZ16_Norm:
		return VK_FORMAT_R16G16B16_SNORM;
	case rf::EVertexAttributeFormat::XYZW16_Norm:
		return VK_FORMAT_R16G16B16A16_SNORM;

	case rf::EVertexAttributeFormat::X8_UNorm:
		return VK_FORMAT_R8_UNORM;
	case rf::EVertexAttributeFormat::XY8_UNorm:
		return VK_FORMAT_R8G8_UNORM;
	case rf::EVertexAttributeFormat::XYZ8_UNorm:
		return VK_FORMAT_R8G8B8_UNORM;
	case rf::EVertexAttributeFormat::XYZW8_UNorm:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case rf::EVertexAttributeFormat::X16_UNorm:
		return VK_FORMAT_R16_UNORM;
	case rf::EVertexAttributeFormat::XY16_UNorm:
		return VK_FORMAT_R16G16_UNORM;
	case rf::EVertexAttributeFormat::XYZ16_UNorm:
		return VK_FORMAT_R16G16B16_UNORM;
	case rf::EVertexAttributeFormat::XYZW16_UNorm:
		return VK_FORMAT_R16G16B16A16_UNORM;

	case rf::EVertexAttributeFormat::X8_UInt:
		return VK_FORMAT_R8_UINT;
	case rf::EVertexAttributeFormat::X8_Int:
		return VK_FORMAT_R8_SINT;
	case rf::EVertexAttributeFormat::XY8_UInt:
		return VK_FORMAT_R8G8_UINT;
	case rf::EVertexAttributeFormat::XY8_Int:
		return VK_FORMAT_R8G8_SINT;
	case rf::EVertexAttributeFormat::XYZ8_UInt:
		return VK_FORMAT_R8G8B8_UINT;
	case rf::EVertexAttributeFormat::XYZ8_Int:
		return VK_FORMAT_R8G8B8_SINT;
	case rf::EVertexAttributeFormat::XYZW8_UInt:
		return VK_FORMAT_R8G8B8A8_UINT;
	case rf::EVertexAttributeFormat::XYZW8_Int:
		return VK_FORMAT_R8G8B8A8_SINT;

	case rf::EVertexAttributeFormat::X16_Float:
		return VK_FORMAT_R16_SFLOAT;
	case rf::EVertexAttributeFormat::XY16_Float:
		return VK_FORMAT_R16G16_SFLOAT;
	case rf::EVertexAttributeFormat::XYZ16_Float:
		return VK_FORMAT_R16G16B16_SFLOAT;
	case rf::EVertexAttributeFormat::XYZW16_Float:
		return VK_FORMAT_R16G16B16A16_SFLOAT;
	case rf::EVertexAttributeFormat::X32_Float:
		return VK_FORMAT_R32_SFLOAT;
	case rf::EVertexAttributeFormat::XY32_Float:
		return VK_FORMAT_R32G32_SFLOAT;
	case rf::EVertexAttributeFormat::XYZ32_Float:
		return VK_FORMAT_R32G32B32_SFLOAT;
	case rf::EVertexAttributeFormat::XYZW32_Float:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case rf::EVertexAttributeFormat::X64_Float:
		return VK_FORMAT_R64_SFLOAT;
	case rf::EVertexAttributeFormat::XY64_Float:
		return VK_FORMAT_R64G64_SFLOAT;
	case rf::EVertexAttributeFormat::XYZ64_Float:
		return VK_FORMAT_R64G64B64_SFLOAT;
	case rf::EVertexAttributeFormat::XYZW64_Float:
		return VK_FORMAT_R64G64B64A64_SFLOAT;

	case rf::EVertexAttributeFormat::X16_Int:
		return VK_FORMAT_R16_SINT;
	case rf::EVertexAttributeFormat::XY16_Int:
		return VK_FORMAT_R16G16_SINT;
	case rf::EVertexAttributeFormat::XYZ16_Int:
		return VK_FORMAT_R16G16B16_SINT;
	case rf::EVertexAttributeFormat::XYZW16_Int:
		return VK_FORMAT_R16G16B16A16_SINT;
	case rf::EVertexAttributeFormat::X32_Int:
		return VK_FORMAT_R32_SINT;
	case rf::EVertexAttributeFormat::XY32_Int:
		return VK_FORMAT_R32G32_SINT;
	case rf::EVertexAttributeFormat::XYZ32_Int:
		return VK_FORMAT_R32G32B32_SINT;
	case rf::EVertexAttributeFormat::XYZW32_Int:
		return VK_FORMAT_R32G32B32A32_SINT;
	case rf::EVertexAttributeFormat::X64_Int:
		return VK_FORMAT_R64_SINT;
	case rf::EVertexAttributeFormat::XY64_Int:
		return VK_FORMAT_R64G64_SINT;
	case rf::EVertexAttributeFormat::XYZ64_Int:
		return VK_FORMAT_R64G64B64_SINT;
	case rf::EVertexAttributeFormat::XYZW64_Int:
		return VK_FORMAT_R64G64B64A64_SINT;

	case rf::EVertexAttributeFormat::X16_UInt:
		return VK_FORMAT_R16_UINT;
	case rf::EVertexAttributeFormat::XY16_UInt:
		return VK_FORMAT_R16G16_UINT;
	case rf::EVertexAttributeFormat::XYZ16_UInt:
		return VK_FORMAT_R16G16B16_UINT;
	case rf::EVertexAttributeFormat::XYZW16_UInt:
		return VK_FORMAT_R16G16B16A16_UINT;
	case rf::EVertexAttributeFormat::X32_UInt:
		return VK_FORMAT_R32_UINT;
	case rf::EVertexAttributeFormat::XY32_UInt:
		return VK_FORMAT_R32G32_UINT;
	case rf::EVertexAttributeFormat::XYZ32_UInt:
		return VK_FORMAT_R32G32B32_UINT;
	case rf::EVertexAttributeFormat::XYZW32_UInt:
		return VK_FORMAT_R32G32B32A32_UINT;
	case rf::EVertexAttributeFormat::X64_UInt:
		return VK_FORMAT_R64_UINT;
	case rf::EVertexAttributeFormat::XY64_UInt:
		return VK_FORMAT_R64G64_UINT;
	case rf::EVertexAttributeFormat::XYZ64_UInt:
		return VK_FORMAT_R64G64B64_UINT;
	case rf::EVertexAttributeFormat::XYZW64_UInt:
		return VK_FORMAT_R64G64B64A64_UINT;

	default:
		return VK_FORMAT_UNDEFINED;
	}
}

auto rf::api::ToVk(rf::ETextureFormat format)->VkFormat {
	switch (format) {
	case rf::ETextureFormat::D32_Float:
		return VK_FORMAT_D32_SFLOAT;
	case rf::ETextureFormat::D24_Float_S8_UInt:
		return VK_FORMAT_D24_UNORM_S8_UINT;

	case rf::ETextureFormat::R8_UNorm:
		return VK_FORMAT_R8_UNORM;
	case rf::ETextureFormat::R16_UNorm:
		return VK_FORMAT_R16_UNORM;
	case rf::ETextureFormat::R8G8_UNorm:
		return VK_FORMAT_R8G8_UNORM;
	case rf::ETextureFormat::R8G8B8A8_UNorm:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case rf::ETextureFormat::A2R10G10B10_UNorm:
		return VK_FORMAT_A2R10G10B10_UNORM_PACK32;

	case rf::ETextureFormat::R16_Float:
		return VK_FORMAT_R16_SFLOAT;
	case rf::ETextureFormat::R32_Float:
		return VK_FORMAT_R32_SFLOAT;
	case rf::ETextureFormat::R16G16_Float:
		return VK_FORMAT_R16G16_SFLOAT;
	case rf::ETextureFormat::R16G16B16A16_Float:
		return VK_FORMAT_R16G16B16A16_SFLOAT;

	default:
		return VK_FORMAT_UNDEFINED;
	}
}