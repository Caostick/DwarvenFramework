#include "VkCommandBuffer.h"
#include "VkRenderPass.h"
#include "VkPipeline.h"
#include "VkDebug.h"

#include <DwarvenCore/Assert.h>

vk::CommandBuffer::CommandBuffer()
	: m_VkCommandBuffer(VK_NULL_HANDLE) 
	, m_CurrentRenderPass(nullptr) {}

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

void vk::CommandBuffer::CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset /*= 0*/, uint32 dstOffset /*= 0*/) {
	VkBufferCopy copyRegion = {};
	copyRegion.size = range;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;

	vk::API::CmdCopyBuffer(m_VkCommandBuffer, src, dst, 1, &copyRegion);
}

void vk::CommandBuffer::CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height) {
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

void vk::CommandBuffer::EndRenderPass() {
	m_CurrentRenderPass = nullptr;

	vk::API::CmdEndRenderPass(m_VkCommandBuffer);
}

void vk::CommandBuffer::SetPipeline(vk::Pipeline* pipeline) {
	m_CurrentPipeline = pipeline;
}