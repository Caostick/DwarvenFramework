#pragma once

#include "VkAPI.h"

namespace rf {
	namespace api {
		struct Texture {
			Texture() = default;
			Texture(const Texture&) = default;
			Texture(Texture&&) = default;
			Texture& operator = (const Texture&) = default;
			Texture& operator = (Texture&&) = default;

			VkImage m_Image = VK_NULL_HANDLE;
			VkImageView m_ImageView = VK_NULL_HANDLE;
			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			VkFormat m_Format = VK_FORMAT_UNDEFINED;
		};
	}
}