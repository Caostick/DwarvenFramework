#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkPass.h"
#endif

#include "AttachmentDefinition.h"

namespace rf {
	struct Pass {
		vk::Pass m_APIData;
	};
}