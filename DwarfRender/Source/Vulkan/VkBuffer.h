#pragma once

#include "VkAPI.h"

#include <DwarfRender/Buffer.h>

namespace vk {
	class RenderCore;
}

namespace vk {
	class Buffer : public df::Buffer {
	public:
		Buffer(vk::RenderCore& renderCore);
		
		virtual ~Buffer() override;

	public:
		auto GetVkBuffer() const->VkBuffer;
		auto GetVkBufferUsageFlags() const->VkBufferUsageFlags;
		auto GetVkDeviceSize() const->VkDeviceSize;
		auto GetVkDeviceMemory() const->VkDeviceMemory;
		auto GetVkMemoryPropertyFlags() const->VkMemoryPropertyFlags;

	private:
		vk::RenderCore& m_RenderCore;

		VkBuffer m_VkBuffer;
		VkBufferUsageFlags m_VkBufferUsageFlags;
		VkDeviceSize m_VkBufferSize;
		VkDeviceMemory m_VkMemory;
		VkMemoryPropertyFlags m_VkMemoryPropertyFlags;
	};
}