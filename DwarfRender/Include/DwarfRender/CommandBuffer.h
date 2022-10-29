#pragma once

#include <DwarvenCore/Types.h>

namespace df {
	class RenderPass;
	class GraphicsPipeline;
	class ParameterSet;
	class Mesh;
}

namespace df {
	class CommandBuffer {
	public:
		virtual ~CommandBuffer() {}

		virtual void BeginRenderPass(RenderPass* renderPass) = 0;
		virtual void EndRenderPass() = 0;

		virtual void BindPipeline(GraphicsPipeline* pipeline) = 0;
		virtual bool BindParameterSet(df::ParameterSet* parameterSet) = 0;

		virtual void Draw(uint32 vertexCount) = 0;
		virtual void Draw(Mesh* mesh, uint32 instanceCount = 1) = 0;
	};
}