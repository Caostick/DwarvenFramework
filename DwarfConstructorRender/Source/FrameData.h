#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkFrameData.h"
#endif

#include "CommandBuffer.h"

namespace rf {
	struct FrameData {
		api::FrameData m_APIData;

		rf::CommandBuffer m_CommandBuffer;
	};
}