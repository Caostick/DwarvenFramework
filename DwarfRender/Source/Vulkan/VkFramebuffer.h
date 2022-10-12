#pragma once

#include "VkAPI.h"

namespace vk {
	struct Framebuffer {
		Framebuffer() = default;
		Framebuffer(const Framebuffer&) = default;
		Framebuffer(Framebuffer&&) = default;
		Framebuffer& operator = (const Framebuffer&) = default;
		Framebuffer& operator = (Framebuffer&&) = default;

		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
		VkExtent2D m_Extends = {0, 0};
	};
}