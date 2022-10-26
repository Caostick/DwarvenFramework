#pragma once

#include <DwarvenCore/Types.h>

namespace df {
	class RenderPass;
	class Pipeline;
	class ParameterSet;
}

namespace df {
	class CommandBuffer {
	public:
		virtual ~CommandBuffer() {}

		virtual void BeginRenderPass(RenderPass* renderPass) = 0;
		virtual void EndRenderPass() = 0;

		virtual void BindPipeline(Pipeline* pipeline) = 0;
		virtual bool BindParameterSet(df::ParameterSet* parameterSet) = 0;

		virtual void Draw(uint32 vertexCount) = 0;
	};
}