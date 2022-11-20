#pragma once

#include <DwarfConstructorRender/RenderPassRegistry.h>

namespace rf {
	class ParamSetDeclarator;
	class ParamSet;

	namespace sg {
		class GraphicsPipelineGenerator;
	}
}

namespace rf {
	class RenderPass {
	public:
		virtual ~RenderPass();

		// RenderTargets using in shaders
		using Inputs = df::TypeList<>;

		// RenderTargets using as pipeline output
		using Outputs = df::TypeList<>;

		// PipelineGenerator that will generate shaders for materials processed by RenderPass
		using PipelineGeneratorType = sg::GraphicsPipelineGenerator;

		// RenderPass name for render debug
		static const char* Name;

		// Initializes params definition for RenderPass type
		static void InitParams(rf::ParamSetDeclarator& params);

		// Updates RenderPass instance params
		virtual void UpdateParams(rf::ParamSet& paramSet);
	};
}