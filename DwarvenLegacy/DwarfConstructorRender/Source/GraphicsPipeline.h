#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkGraphicsPipeline.h"
#endif

#include <DwarfConstructorRender/DepthState.h>
#include <DwarfConstructorRender/RasterizationState.h>
#include <DwarfConstructorRender/BlendState.h>
#include <DwarfConstructorRender/VertexAttribute.h>
#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

namespace rf {
	class ParamSetDefinition;
}

namespace rf {
	struct GraphicsPipeline {
		api::GraphicsPipeline m_APIData;

		df::String m_Name;
		//df::Vector<rf::VertexAttributeLocation> m_VertexAttributes;
		//df::Vector<const rf::ParamSetDefinition*> m_ParameterSets;
		//df::Vector<uint32> m_VSCode;
		//df::Vector<uint32> m_FSCode;

		//DepthState m_DepthState;
		//RasterizationState m_RasterizationState;
		//BlendState m_BlendState;

		rf::VertexShaderModuleId m_VertexShader = nullptr;
		rf::FragmentShaderModuleId m_FragmentShader = nullptr;
	};
}