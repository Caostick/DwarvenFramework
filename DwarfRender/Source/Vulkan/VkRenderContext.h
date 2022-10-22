#pragma once

#include <DwarvenCore/Types.h>

namespace vk {
	class CommandBuffer;
}

namespace vk {
	struct RenderContext {
		RenderContext() = default;
		RenderContext(const RenderContext&) = default;
		RenderContext(RenderContext&&) = default;
		RenderContext& operator = (const RenderContext&) = default;
		RenderContext& operator = (RenderContext&&) = default;

		vk::CommandBuffer* m_CommandBuffer = nullptr;
		uint32 m_FrameIndex = 0;
	};
}