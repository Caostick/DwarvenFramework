#pragma once

#include "VkAPI.h"

namespace rf {
	namespace api {
		class ScopedRenderEvent {
		public:
			ScopedRenderEvent() = delete;
			ScopedRenderEvent(VkCommandBuffer rcb, const char* name);
			ScopedRenderEvent(const ScopedRenderEvent&) = delete;
			ScopedRenderEvent(ScopedRenderEvent&& other) noexcept;
			ScopedRenderEvent& operator = (ScopedRenderEvent&& other) noexcept;
			ScopedRenderEvent& operator = (const ScopedRenderEvent&) = delete;
			~ScopedRenderEvent();

		private:
			VkCommandBuffer m_VkCommandBuffer;
		};
	}
}