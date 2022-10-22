#pragma once

#include <DwarvenCore/Types.h>

namespace df {
	class RenderPass;
	class Pipeline;
}

namespace df {
	class CommandBuffer {
	public:
		virtual ~CommandBuffer() {}

		virtual void BeginRenderPass(RenderPass* renderPass) = 0;
		virtual void EndRenderPass() = 0;

		virtual void SetPipeline(Pipeline* pipeline) = 0;

		virtual void Draw(uint32 vertexCount) = 0;
	};
}