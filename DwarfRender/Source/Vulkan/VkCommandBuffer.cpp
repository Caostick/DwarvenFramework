#include "VkCommandBuffer.h"
#include "VkRenderPass.h"
#include "VkParameterSet.h"
#include "VkPipeline.h"
#include "VkTexture.h"
#include "VkDebug.h"
#include "VkScopedRenderEvent.h"

#include <DwarvenCore/Assert.h>

namespace {
	void ToVk(vk::EImageLayout layout, VkImageLayout& vkLayout, VkAccessFlags& accessFlags, VkPipelineStageFlags& stageFlags) {
		switch (layout) {
		case vk::EImageLayout::Undefined:
			vkLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			accessFlags = 0;
			stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			break;
		case vk::EImageLayout::General:
			vkLayout = VK_IMAGE_LAYOUT_GENERAL;
			accessFlags = 0;
			stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			break;
		case vk::EImageLayout::ColorAttachment:
			vkLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			break;
		case vk::EImageLayout::ColorReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_SHADER_READ_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case vk::EImageLayout::DepthStencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			break;
		case vk::EImageLayout::DepthStencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case vk::EImageLayout::DepthReadOnlyStencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			break;
		case vk::EImageLayout::DepthAttachmentStencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case vk::EImageLayout::DepthAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			break;
		case vk::EImageLayout::DepthReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case vk::EImageLayout::StencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			break;
		case vk::EImageLayout::StencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case vk::EImageLayout::TransferSrc:
			vkLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			accessFlags = VK_ACCESS_TRANSFER_READ_BIT;
			stageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		case vk::EImageLayout::TransferDst:
			vkLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			accessFlags = VK_ACCESS_TRANSFER_WRITE_BIT;
			stageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		default:
			break;
		}
	}
}

vk::CommandBuffer::CommandBuffer()
	: m_VkCommandBuffer(VK_NULL_HANDLE) 
	, m_CurrentRenderPass(nullptr) 
	, m_CurrentPipeline(nullptr) {
}

vk::CommandBuffer::~CommandBuffer() {

}

void vk::CommandBuffer::BeginRenderPass(df::RenderPass* renderPass) {
	BeginRenderPass(static_cast<vk::RenderPass*>(renderPass));
}

void vk::CommandBuffer::EndRenderPass() {
	m_CurrentRenderPass = nullptr;

	vk::API::CmdEndRenderPass(m_VkCommandBuffer);
}

void vk::CommandBuffer::SetPipeline(df::Pipeline* pipeline) {
	SetPipeline(static_cast<vk::Pipeline*>(pipeline));
}

bool vk::CommandBuffer::BindParameterSet(df::ParameterSet* parameterSet) {
	return BindParameterSet(static_cast<vk::ParameterSet*>(parameterSet));
}

void vk::CommandBuffer::Draw(uint32 vertexCount) {
	ValidateState();

	vk::API::CmdDraw(m_VkCommandBuffer, vertexCount, 1, 0, 0);
}

auto vk::CommandBuffer::Get() const->VkCommandBuffer {
	return m_VkCommandBuffer;
}

bool vk::CommandBuffer::Init(const df::StringView& name, VkDevice device, VkCommandPool commandPool) {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;
	allocInfo.commandPool = commandPool;

	if (vk::API::AllocateCommandBuffers(device, &allocInfo, &m_VkCommandBuffer) != VK_SUCCESS) {
		DFAssert(false, "Can't allocate CommandBuffer!");

		return false;
	}

	DFVkDebugName(device, m_VkCommandBuffer, name);

	return true;
}

void vk::CommandBuffer::Release(VkDevice device, VkCommandPool commandPool) {
	vk::API::FreeCommandBuffers(device, commandPool, 1, &m_VkCommandBuffer);
}

bool vk::CommandBuffer::Begin() {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vk::API::BeginCommandBuffer(m_VkCommandBuffer, &beginInfo) != VK_SUCCESS) {
		DFAssert(false, "Can't begin recording CommandBuffer!");
		return false;
	}

	return true;
}

void vk::CommandBuffer::End() {
	if (vk::API::EndCommandBuffer(m_VkCommandBuffer) != VK_SUCCESS) {
		DFAssert(false, "Failed to record CommandBuffer!");
	}

	m_CurrentPipeline = nullptr;
	m_CurrentRenderPass = nullptr;
}

void vk::CommandBuffer::Submit(VkQueue queue) {
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_VkCommandBuffer;

	vk::API::QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
}

void vk::CommandBuffer::Wait(VkQueue queue) {
	vk::API::QueueWaitIdle(queue);
}

void vk::CommandBuffer::ImageLayoutTransition(VkImage image, uint32 mipCount, vk::EImageLayout oldLayout, vk::EImageLayout newLayout, int32 mip /*= -1*/, bool isDepth /*= false*/, bool isStencil /*= false*/) {
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.baseMipLevel = (mip < 0) ? 0 : mip;
	barrier.subresourceRange.levelCount = (mip < 0) ? mipCount : mip;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;
	barrier.subresourceRange.aspectMask = (!isDepth && !isStencil) ? VK_IMAGE_ASPECT_COLOR_BIT : 0;
	barrier.subresourceRange.aspectMask |= isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
	barrier.subresourceRange.aspectMask |= isStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

	VkPipelineStageFlags sourceStage = 0;
	VkPipelineStageFlags destinationStage = 0;

	ToVk(oldLayout, barrier.oldLayout, barrier.srcAccessMask, sourceStage);
	ToVk(newLayout, barrier.newLayout, barrier.dstAccessMask, destinationStage);

	vk::API::CmdPipelineBarrier(m_VkCommandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void vk::CommandBuffer::ImageLayoutTransition(vk::Texture* texture, vk::EImageLayout oldLayout, vk::EImageLayout newLayout, int32 mip /*= -1*/) {
	const bool isDepth = texture->IsDepth();
	const bool isStencil = texture->IsStencil();
	const auto mipCount = texture->GetMips();
	const auto image = texture->GetVkImage();

	ImageLayoutTransition(image, mipCount, oldLayout, newLayout, mip, isDepth, isStencil);
}

void vk::CommandBuffer::CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset /*= 0*/, uint32 dstOffset /*= 0*/) {
	VkBufferCopy copyRegion = {};
	copyRegion.size = range;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;

	vk::API::CmdCopyBuffer(m_VkCommandBuffer, src, dst, 1, &copyRegion);
}

void vk::CommandBuffer::CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height, int32 widthOffset /*= 0*/, int32 heightOffset /*= 0*/) {
	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { widthOffset, heightOffset, 0 };
	copyRegion.imageExtent = { width, height, 1 };

	vk::API::CmdCopyBufferToImage(m_VkCommandBuffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}

void vk::CommandBuffer::GenerateMips(vk::Texture* texture) {
	DFScopedRenderEvent((*this), "Generate Mips");

	const uint32 mipLevels = texture->GetMips();
	if (mipLevels < 2) {
		return;
	}

	const VkImage vkImage = texture->GetVkImage();
	const uint32 width = texture->GetWidth();
	const uint32 height = texture->GetHeight();
	const bool isDepth = texture->IsDepth();
	const bool isStencil = texture->IsStencil();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = vkImage;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.aspectMask |= isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
	barrier.subresourceRange.aspectMask |= isStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = width;
	int32_t mipHeight = height;

	for (uint32_t i = 0; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i;
		barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		vk::API::CmdPipelineBarrier(m_VkCommandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);
	}

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vk::API::CmdPipelineBarrier(m_VkCommandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vk::API::CmdBlitImage(m_VkCommandBuffer,
			vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vk::API::CmdPipelineBarrier(m_VkCommandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vk::API::CmdPipelineBarrier(m_VkCommandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

auto vk::CommandBuffer::ScopedRenderEvent(const char* name)->vk::ScopedRenderEvent {
	return vk::ScopedRenderEvent(m_VkCommandBuffer, name);
}

void vk::CommandBuffer::BeginRenderPass(vk::RenderPass* renderPass) {
	m_CurrentRenderPass = renderPass;

	renderPass->Validate();

	VkRenderPass vkRenderPass = renderPass->GetVkRenderPass();
	VkFramebuffer vkFramebuffer = renderPass->GetVkFramebuffer();
	const auto& vkClearValues = renderPass->GetVkClearValues();
	const auto& vkExtents = renderPass->GetVkExtents();

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = vkRenderPass;
	renderPassInfo.framebuffer = vkFramebuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = vkExtents;
	renderPassInfo.clearValueCount = uint32(vkClearValues.size());
	renderPassInfo.pClearValues = vkClearValues.data();

	VkViewport viewport = {};
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = float(vkExtents.width);
	viewport.height = float(vkExtents.height);

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = vkExtents;

	vk::API::CmdBeginRenderPass(m_VkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vk::API::CmdSetViewport(m_VkCommandBuffer, 0, 1, &viewport);
	vk::API::CmdSetScissor(m_VkCommandBuffer, 0, 1, &scissor);
}

void vk::CommandBuffer::SetPipeline(vk::Pipeline* pipeline) {
	m_CurrentPipeline = pipeline;
}

bool vk::CommandBuffer::BindParameterSet(vk::ParameterSet* parameterSet) {
	DFAssert(m_CurrentPipeline != nullptr, "Can't bind parameter set - Pipeline is not set!");

	const int32 slotIdx = m_CurrentPipeline->GetParameterSetSlot(parameterSet);
	if (slotIdx == -1) {
		return false;
	}

	const VkPipelineLayout vkPipelineLayout = m_CurrentPipeline->GetVkPipelineLayout();
	const VkDescriptorSet vkDescriptorSet = parameterSet->GetVkDescriptorSet();

	vk::API::CmdBindDescriptorSets(m_VkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, uint32(slotIdx), 1, &vkDescriptorSet, 0, nullptr);

	return true;
}

void vk::CommandBuffer::ValidateState() {

}