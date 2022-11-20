#pragma once

#include "VkAPI.h"

#include <DwarvenCore/Vector.h>

namespace vk {
	struct Pass {
		Pass() = default;
		Pass(const Pass&) = default;
		Pass(Pass&&) = default;
		Pass& operator = (const Pass&) = default;
		Pass& operator = (Pass&&) = default;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		df::Vector<VkClearValue> m_ClearValues;
		uint32 m_ColorAttachmentCount = 0;
	};
}