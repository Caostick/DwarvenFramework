#pragma once

#include <DwarvenCore/TypeList.h>

namespace rf {
	struct RenderSetup {
		using ShaderGraphType = void;

		using VertexAttributes = df::TypeList<>;
		using RenderTargets = df::TypeList<>;
		using RenderPasses = df::TypeList<>;
		using RenderViews = df::TypeList<>;

		using GlobalShaderParams = df::TypeList<>;
		using ViewShaderParams = df::TypeList<>;
	};
}