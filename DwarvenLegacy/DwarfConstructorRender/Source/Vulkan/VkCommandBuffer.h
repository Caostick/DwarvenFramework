#pragma once

#include "VkAPI.h"
#include "VkScopedRenderEvent.h"

#include <DwarvenCore/StringView.h>

namespace rf {
	namespace api {
		struct CommandBuffer {
			CommandBuffer();

			auto Get() const->VkCommandBuffer;

			bool Init(const df::StringView& name, VkDevice device, VkCommandPool commandPool);
			void Release(VkDevice device, VkCommandPool commandPool);

			void Submit(VkQueue queue);
			void Wait(VkQueue queue);

			void CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset = 0, uint32 dstOffset = 0);
			void CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height);

			auto ScopedRenderEvent(const char* name)->rf::api::ScopedRenderEvent;

			VkCommandBuffer m_VkCommandBuffer;
			VkFence m_VkFence;
		};
	}
}

#if DF_USE_DEBUG_MARKERS
#define DFScopedRenderEvent(rcb, name) const rf::api::ScopedRenderEvent _df_dbg_marker_ = rcb.GetAPIData().ScopedRenderEvent(name)
#else
#define DFScopedRenderEvent(rcb, name)
#endif