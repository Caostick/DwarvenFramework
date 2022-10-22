#pragma once

#include <DwarfRender/BlendState.h>
#include <DwarfRender/DepthState.h>
#include <DwarfRender/RasterizationState.h>

#include <DwarvenCore/Types.h>

namespace vk {
	class ParameterSetDefinition;

	struct VertexAttribute;
}

namespace vk {
	struct ShaderCompileInfo {
		df::Vector<uint32> m_SpirVCode;
		df::Vector<const ParameterSetDefinition*> m_ParameterSets;
		df::Vector<const VertexAttribute*> m_Attributes;
		df::DepthState m_DepthState;
		df::RasterizationState m_RasterizationState;
		df::BlendState m_BlendState;
	};
}