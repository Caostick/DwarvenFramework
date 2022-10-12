#include "../CommandBuffer.h"

#include "VkRenderCore.h"
#include "VkHelper.h"
#include "VkDebug.h"

namespace {
	void ToVk(rf::EImageLayout layout, VkImageLayout& vkLayout, VkAccessFlags& accessFlags) {
		switch (layout) {
		case rf::EImageLayout::Undefined:
			vkLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			accessFlags = 0;
			break;
		case rf::EImageLayout::General:
			vkLayout = VK_IMAGE_LAYOUT_GENERAL;
			accessFlags = 0;
			break;
		case rf::EImageLayout::ColorAttachment:
			vkLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::ColorReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_SHADER_READ_BIT;
			break;
		case rf::EImageLayout::DepthStencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::DepthStencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
		case rf::EImageLayout::DepthReadOnlyStencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::DepthAttachmentStencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::DepthAttachment:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::DepthReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
		case rf::EImageLayout::StencilAttachment:
			vkLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case rf::EImageLayout::StencilReadOnly:
			vkLayout = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
			accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
		case rf::EImageLayout::TransferSrc:
			vkLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			accessFlags = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case rf::EImageLayout::TransferDst:
			vkLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			accessFlags = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		default:
			break;
		}
	}
}


auto rf::CommandBuffer::GetAPIData()->rf::api::CommandBuffer& {
	return m_APIData;
}

bool rf::CommandBuffer::Begin() {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vk::API::BeginCommandBuffer(m_APIData.m_VkCommandBuffer, &beginInfo) != VK_SUCCESS) {
		DFAssert(false, "Can't begin recording CommandBuffer!");
		return false;
	}

	return true;
}

void rf::CommandBuffer::End() {
	if (vk::API::EndCommandBuffer(m_APIData.m_VkCommandBuffer) != VK_SUCCESS) {
		DFAssert(false, "Failed to record CommandBuffer!");
	}
}

void rf::CommandBuffer::CopyBuffer(rf::BufferId srcBuffer, rf::BufferId dstBuffer, uint32 range) {
	VkBufferCopy copyRegion = {};
	copyRegion.size = range;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;

	vk::API::CmdCopyBuffer(m_APIData.m_VkCommandBuffer, srcBuffer->m_APIData.m_Buffer, dstBuffer->m_APIData.m_Buffer, 1, &copyRegion);
}

void rf::CommandBuffer::CopyBufferToImage(rf::BufferId srcBuffer, rf::TextureId dstImage, uint32 width, uint32 height) {

	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { 0, 0, 0 };
	copyRegion.imageExtent = { width, height, 0 };

	vk::API::CmdCopyBufferToImage(m_APIData.m_VkCommandBuffer, srcBuffer->m_APIData.m_Buffer, dstImage->m_APIData.m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}

void rf::CommandBuffer::InitializeColorAttachment(rf::TextureId texture) {
	const rf::api::Texture& tex = texture->m_APIData;

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = tex.m_Image;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
	barrier.subresourceRange.aspectMask = (!vk::IsDepth(tex.m_Format) && !vk::IsStencil(tex.m_Format)) ? VK_IMAGE_ASPECT_COLOR_BIT : 0;
	barrier.subresourceRange.aspectMask |= vk::IsDepth(tex.m_Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
	barrier.subresourceRange.aspectMask |= vk::IsStencil(tex.m_Format) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

	VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	vk::API::CmdPipelineBarrier(m_APIData.m_VkCommandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void rf::CommandBuffer::TransitionImageLayout(rf::TextureId texture, rf::EImageLayout oldLayout, rf::EImageLayout newLayout) {
	const rf::api::Texture& tex = texture->m_APIData;

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = tex.m_Image;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;
	barrier.subresourceRange.aspectMask = (!vk::IsDepth(tex.m_Format) && !vk::IsStencil(tex.m_Format)) ? VK_IMAGE_ASPECT_COLOR_BIT : 0;
	barrier.subresourceRange.aspectMask |= vk::IsDepth(tex.m_Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
	barrier.subresourceRange.aspectMask |= vk::IsStencil(tex.m_Format) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

	VkPipelineStageFlags sourceStage = (oldLayout == rf::EImageLayout::TransferDst) ? VK_PIPELINE_STAGE_TRANSFER_BIT : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destinationStage = (newLayout == rf::EImageLayout::TransferDst) ? VK_PIPELINE_STAGE_TRANSFER_BIT : VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

	if (oldLayout == EImageLayout::ColorAttachment) {
		sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	} else if (oldLayout == EImageLayout::DepthStencilAttachment) {
		sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}

	if (newLayout == EImageLayout::ColorAttachment) {
		destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	} else if (newLayout == EImageLayout::DepthStencilAttachment) {
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	} else if (newLayout == EImageLayout::DepthStencilReadOnly) {
		destinationStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	}

	ToVk(oldLayout, barrier.oldLayout, barrier.srcAccessMask);
	ToVk(newLayout, barrier.newLayout, barrier.dstAccessMask);

	vk::API::CmdPipelineBarrier(m_APIData.m_VkCommandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void rf::CommandBuffer::BeginRenderPass(rf::PassId renderPass, rf::FramebufferId framebuffer) {
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass->m_APIData.m_RenderPass;
	renderPassInfo.framebuffer = framebuffer->m_APIData.m_Framebuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = framebuffer->m_APIData.m_Extends;
	renderPassInfo.clearValueCount = uint32(renderPass->m_APIData.m_ClearValues.size());
	renderPassInfo.pClearValues = renderPass->m_APIData.m_ClearValues.data();

	VkViewport viewport = {};
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = float(framebuffer->m_APIData.m_Extends.width);
	viewport.height = float(framebuffer->m_APIData.m_Extends.height);

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = framebuffer->m_APIData.m_Extends;

	vk::API::CmdBeginRenderPass(m_APIData.m_VkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vk::API::CmdSetViewport(m_APIData.m_VkCommandBuffer, 0, 1, &viewport);
	vk::API::CmdSetScissor(m_APIData.m_VkCommandBuffer, 0, 1, &scissor);
}

void rf::CommandBuffer::EndRenderPass() {
	vk::API::CmdEndRenderPass(m_APIData.m_VkCommandBuffer);
}

void rf::CommandBuffer::SetLineWidth(float width) {
	vk::API::CmdSetLineWidth(m_APIData.m_VkCommandBuffer, width);
}

void rf::CommandBuffer::BindPipeline(rf::PipelineId pipeline) {
	vk::API::CmdBindPipeline(m_APIData.m_VkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_Handle);
}

void rf::CommandBuffer::BindDescriptorSet(rf::PipelineLayoutId pipelineLayout, rf::DescriptorSetId descriptorSet) {
	vk::API::CmdBindDescriptorSets(m_APIData.m_VkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->m_Handle, descriptorSet->m_Id, 1, &descriptorSet->m_APIData.m_Handle, 0, nullptr);
}

void rf::CommandBuffer::BindVertexBuffer(rf::BufferId buffer, uint32 binding, uint32 offset /*= 0*/) {
	VkBuffer vertexBuffers[] = { buffer->m_APIData.m_Buffer };
	VkDeviceSize offsets[] = { offset };
	vk::API::CmdBindVertexBuffers(m_APIData.m_VkCommandBuffer, binding, 1, vertexBuffers, offsets);
}

void rf::CommandBuffer::BindIndexBuffer(rf::BufferId buffer) {
	vk::API::CmdBindIndexBuffer(m_APIData.m_VkCommandBuffer, buffer->m_APIData.m_Buffer, 0, VK_INDEX_TYPE_UINT32);
}

void rf::CommandBuffer::Draw(uint32 vertexCount) {
	vk::API::CmdDraw(m_APIData.m_VkCommandBuffer, vertexCount, 1, 0, 0);
}

void rf::CommandBuffer::Draw(uint32 vertexOffset, uint32 vertexCount) {
	vk::API::CmdDraw(m_APIData.m_VkCommandBuffer, vertexCount, 1, vertexOffset, 0);
}

void rf::CommandBuffer::DrawIndexed(uint32 indexCount) {
	vk::API::CmdDrawIndexed(m_APIData.m_VkCommandBuffer, indexCount, 1, 0, 0, 0);
}

void rf::CommandBuffer::DrawInstanced(uint32 vertexCount, uint32 firstInstance, uint32 instanceCount) {
	vk::API::CmdDraw(m_APIData.m_VkCommandBuffer, vertexCount, instanceCount, 0, firstInstance);
}

void rf::CommandBuffer::DrawInstancedIndexed(uint32 indexCount, uint32 firstInstance, uint32 instanceCount) {
	vk::API::CmdDrawIndexed(m_APIData.m_VkCommandBuffer, indexCount, instanceCount, 0, 0, firstInstance);
}

void rf::CommandBuffer::SetScissor(uint32 x, uint32 y, uint32 width, uint32 height) {
	VkRect2D rect;
	rect.offset.x = x;
	rect.offset.y = y;
	rect.extent.width = width;
	rect.extent.height = height;

	vk::API::CmdSetScissor(m_APIData.m_VkCommandBuffer, 0, 1, &rect);
}



















rf::api::CommandBuffer::CommandBuffer()
	: m_VkCommandBuffer(VK_NULL_HANDLE)
	, m_VkFence(VK_NULL_HANDLE) {}

auto rf::api::CommandBuffer::Get() const->VkCommandBuffer {
	return m_VkCommandBuffer;
}

bool rf::api::CommandBuffer::Init(const df::StringView& name, VkDevice device, VkCommandPool commandPool) {
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

void rf::api::CommandBuffer::Release(VkDevice device, VkCommandPool commandPool) {
	vk::API::FreeCommandBuffers(device, commandPool, 1, &m_VkCommandBuffer);
}

void rf::api::CommandBuffer::Submit(VkQueue queue) {
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_VkCommandBuffer;

	vk::API::QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
}

void rf::api::CommandBuffer::Wait(VkQueue queue) {
	vk::API::QueueWaitIdle(queue);
}

void rf::api::CommandBuffer::CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset /*= 0*/, uint32 dstOffset /*= 0*/) {
	VkBufferCopy copyRegion = {};
	copyRegion.size = range;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;

	vk::API::CmdCopyBuffer(m_VkCommandBuffer, src, dst, 1, &copyRegion);
}

void rf::api::CommandBuffer::CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height) {
	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { 0, 0, 0 };
	copyRegion.imageExtent = { width, height, 1 };

	vk::API::CmdCopyBufferToImage(m_VkCommandBuffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}

auto rf::api::CommandBuffer::ScopedRenderEvent(const char* name)->rf::api::ScopedRenderEvent {
	return rf::api::ScopedRenderEvent(m_VkCommandBuffer, name);
}