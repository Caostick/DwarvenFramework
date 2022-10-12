#pragma once

#include <DwarvenCore/Types.h>

namespace rf {
	class CommandBuffer;
}

namespace rf {
	struct RenderContext {
		RenderContext() = default;
		RenderContext(const RenderContext&) = default;
		RenderContext(RenderContext&&) = default;
		RenderContext& operator = (const RenderContext&) = default;
		RenderContext& operator = (RenderContext&&) = default;

		rf::CommandBuffer* m_CommandBuffer = nullptr;
		uint32 m_FrameIndex = 0;
	};
}