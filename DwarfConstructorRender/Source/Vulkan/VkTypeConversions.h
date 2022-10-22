#pragma once

#include "VkAPI.h"
#include "../Descriptor.h"
#include "../AttachmentDefinition.h"

#include <DwarfConstructorRender/BlendState.h>
#include <DwarfConstructorRender/RasterizationState.h>
#include <DwarfConstructorRender/DepthState.h>
#include <DwarfConstructorRender/SamplerState.h>
#include <DwarfConstructorRender/PrimitiveTopology.h>
#include <DwarfConstructorRender/VertexAttribute.h>

namespace rf {
	namespace api {
		auto ToVk(rf::EPolygonMode value)->VkPolygonMode;
		auto ToVk(rf::ECullMode value)->VkCullModeFlagBits;
		auto ToVk(rf::EFrontFace value)->VkFrontFace;
		auto ToVk(rf::EBlendFactor value)->VkBlendFactor;
		auto ToVk(rf::EBlendOp value)->VkBlendOp;
		auto ToVk(rf::EDepthCompareOp value)->VkCompareOp;
		auto ToVk(rf::EStencilOp value)->VkStencilOp;
		auto ToVk(rf::EAttachmentLoadOp value)->VkAttachmentLoadOp;
		auto ToVk(rf::EAttachmentStoreOp value)->VkAttachmentStoreOp;
		auto ToVk(rf::EImageLayout value)->VkImageLayout;
		auto ToVk(rf::EFormat value)->VkFormat;
		auto ToVk(rf::EDescriptorType value)->VkDescriptorType;
		auto ToVk(rf::EFilter value)->VkFilter;
		auto ToVk(rf::EMipmapMode value)->VkSamplerMipmapMode;
		auto ToVk(rf::EAddressMode value)->VkSamplerAddressMode;
		auto ToVk(rf::ShaderStageFlags flags)->VkShaderStageFlags;
		auto ToVk(rf::BufferUsageFlags flags)->VkBufferUsageFlags;
		auto ToVk(rf::ImageUsageFlags flags)->VkImageUsageFlags;
		auto ToVk(rf::EPrimitiveTopology topology)->VkPrimitiveTopology;
		auto ToVk(rf::EVertexAttributeFormat format)->VkFormat;
		auto ToVk(rf::ETextureFormat format) ->VkFormat;
	}
}