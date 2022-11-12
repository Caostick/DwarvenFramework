#pragma once

#include "VkAPI.h"
#include "VkCommandBuffer.h"

namespace vk {
	struct FrameData {
		VkFence m_InFlightFence = VK_NULL_HANDLE;
		VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;

		vk::CommandBuffer m_CommandBuffer;
	};
}