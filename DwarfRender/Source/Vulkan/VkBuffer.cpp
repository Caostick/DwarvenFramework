#include "VkBuffer.h"
#include "VkRenderCore.h"

vk::Buffer::Buffer(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore)
	, m_VkBuffer(VK_NULL_HANDLE)
	, m_VkBufferUsageFlags(0)
	, m_VkBufferSize(0)
	, m_VkMemory(VK_NULL_HANDLE)
	, m_VkMemoryPropertyFlags(0) {
}

vk::Buffer::~Buffer() {

}

auto vk::Buffer::GetVkBuffer() const->VkBuffer {
	return m_VkBuffer;
}

auto vk::Buffer::GetVkBufferUsageFlags() const->VkBufferUsageFlags {
	return m_VkBufferUsageFlags;
}

auto vk::Buffer::GetVkDeviceSize() const->VkDeviceSize {
	return m_VkBufferSize;
}

auto vk::Buffer::GetVkDeviceMemory() const->VkDeviceMemory {
	return m_VkMemory;
}

auto vk::Buffer::GetVkMemoryPropertyFlags() const->VkMemoryPropertyFlags {
	return m_VkMemoryPropertyFlags;
}