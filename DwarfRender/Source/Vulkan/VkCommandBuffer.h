#pragma once

#include <DwarfRender/CommandBuffer.h>

#include "VkScopedRenderEvent.h"

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace vk {
	class RenderPass;
	class Pipeline;
}

namespace vk {
	class CommandBuffer : public df::CommandBuffer {
	public:
		CommandBuffer();

		auto Get() const->VkCommandBuffer;

		bool Init(const df::StringView& name, VkDevice device, VkCommandPool commandPool);
		void Release(VkDevice device, VkCommandPool commandPool);

		bool Begin();
		void End();

		void Submit(VkQueue queue);
		void Wait(VkQueue queue);

		void CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset = 0, uint32 dstOffset = 0);
		void CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height);

		auto ScopedRenderEvent(const char* name)->vk::ScopedRenderEvent;

		void BeginRenderPass(vk::RenderPass* renderPass);
		void EndRenderPass();

		void SetPipeline(vk::Pipeline* pipeline);

	private:
		VkCommandBuffer m_VkCommandBuffer;

		vk::RenderPass* m_CurrentRenderPass;
		vk::Pipeline* m_CurrentPipeline;
	};
}

#if DF_USE_DEBUG_MARKERS
#define DFScopedRenderEvent(rcb, name) const vk::ScopedRenderEvent _df_dbg_marker_ = rcb.ScopedRenderEvent(name)
#else
#define DFScopedRenderEvent(rcb, name)
#endif