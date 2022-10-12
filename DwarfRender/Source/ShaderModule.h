#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkShaderModule.h"
#endif

#include <DwarfRender/VertexAttributeRegistry.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>
#include <DwarfRender/BlendState.h>

#include <DwarvenCore/Vector.h>

namespace rf {
	class ParamSetDefinition;
}

namespace rf {
	struct ShaderModule {
		api::ShaderModule m_APIData;

		//df::Vector<uint32> m_SpirVCode;
		df::Vector<const ParamSetDefinition*> m_ParameterSets;
	};

	struct VertexShaderModule : public ShaderModule {
		df::Vector<rf::VertexAttributeLocation> m_Attributes;
		rf::RasterizationState m_RasterizationState;
		rf::DepthState m_DepthState;
	};

	struct FragmentShaderModule : public ShaderModule {
		rf::BlendState m_BlendState;
	};

	struct ComputeShaderModule : public ShaderModule {};
}