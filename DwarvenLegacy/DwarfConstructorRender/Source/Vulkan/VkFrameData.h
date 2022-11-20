#pragma once

#include "VkAPI.h"

namespace rf {
	namespace api {
		struct FrameData {
			VkFence m_InFlightFence = VK_NULL_HANDLE;
			VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
			VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
		};
	}
}