#include "../DescriptorSet.h"
#include "../Buffer.h"
#include "../Sampler.h"
#include "../Texture.h"

#include <DwarvenCore/Assert.h>

namespace {
	struct UpdateParams {
		VkDescriptorBufferInfo m_BufferInfos[vk::Constants::MaxDescriptorCount];
		VkDescriptorImageInfo m_ImageInfos[vk::Constants::MaxDescriptorCount];

		df::Vector<VkWriteDescriptorSet> m_Writes;
	};

	thread_local UpdateParams updateParams;
}

void rf::DescriptorSet::Begin() const {
	updateParams.m_Writes.clear();
}

void rf::DescriptorSet::End() const {
	if (updateParams.m_Writes.empty()) {
		return;
	}

	vk::API::UpdateDescriptorSets(m_APIData.m_Device, uint32(updateParams.m_Writes.size()), updateParams.m_Writes.data(), 0, nullptr);
}

void rf::DescriptorSet::SetStorageBuffer(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size, uint32 offset /*= 0*/) const {
	DFAssert(binding < vk::Constants::MaxDescriptorCount, "Descriptor index is out of range!");

	VkDescriptorBufferInfo& bufferInfo = updateParams.m_BufferInfos[binding];
	bufferInfo.buffer = buffer->m_APIData.m_Buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = size;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = m_APIData.m_Handle;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.dstArrayElement = arrayIdx;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pBufferInfo = &updateParams.m_BufferInfos[binding];

	updateParams.m_Writes.emplace_back(writeDescriptorSet);
}

void rf::DescriptorSet::SetUniformBuffer(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size, uint32 offset /*= 0*/) const {
	DFAssert(binding < vk::Constants::MaxDescriptorCount, "Descriptor index is out of range!");

	VkDescriptorBufferInfo& bufferInfo = updateParams.m_BufferInfos[binding];
	bufferInfo.buffer = buffer->m_APIData.m_Buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = size;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = m_APIData.m_Handle;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.dstArrayElement = arrayIdx;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pBufferInfo = &updateParams.m_BufferInfos[binding];

	updateParams.m_Writes.emplace_back(writeDescriptorSet);
}

void rf::DescriptorSet::SetUniformBufferDynamic(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size) const {
	DFAssert(binding < vk::Constants::MaxDescriptorCount, "Descriptor index is out of range!");

	VkDescriptorBufferInfo& bufferInfo = updateParams.m_BufferInfos[binding];
	bufferInfo.buffer = buffer->m_APIData.m_Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = size;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = m_APIData.m_Handle;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.dstArrayElement = arrayIdx;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pBufferInfo = &updateParams.m_BufferInfos[binding];

	updateParams.m_Writes.emplace_back(writeDescriptorSet);
}

void rf::DescriptorSet::SetCombinedImageSampler(uint32 binding, uint32 arrayIdx, rf::TextureId texture, rf::SamplerId sampler) const {
	DFAssert(binding < vk::Constants::MaxDescriptorCount, "Descriptor index is out of range!");

	const bool isDepthStencil =
		(texture->m_Format == rf::ETextureFormat::D32_Float) ||
		(texture->m_Format == rf::ETextureFormat::D24_Float_S8_UInt);

	VkDescriptorImageInfo& imageInfo = updateParams.m_ImageInfos[binding];
	imageInfo.sampler = sampler->m_Handle;
	imageInfo.imageView = texture->m_APIData.m_ImageView;
	imageInfo.imageLayout = isDepthStencil
		? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
		: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = m_APIData.m_Handle;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.dstArrayElement = arrayIdx;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pImageInfo = &updateParams.m_ImageInfos[binding];

	updateParams.m_Writes.emplace_back(writeDescriptorSet);
}