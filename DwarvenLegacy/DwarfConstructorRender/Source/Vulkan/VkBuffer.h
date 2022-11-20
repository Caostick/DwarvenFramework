#pragma once

#include "VkAPI.h"

namespace rf {
	namespace api {
		struct Buffer {
			Buffer() = default;
			Buffer(const Buffer&) = default;
			Buffer(Buffer&&) = default;
			Buffer& operator = (const Buffer&) = default;
			Buffer& operator = (Buffer&&) = default;

			// Buffer
			VkBuffer m_Buffer = VK_NULL_HANDLE;
			VkBufferUsageFlags m_BufferUsageFlags = 0;

			// Memory
			VkDeviceSize m_BufferSize = 0;
			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			VkMemoryPropertyFlags m_MemoryPropertyFlags = 0;
		};
	}
}