#pragma once

#include <DwarfConstructorRender/ObjectId.h>
#include <DwarfConstructorRender/PrimitiveTopology.h>

namespace rf {
	struct PipelineDescriptor {
		PipelineDescriptor() = default;
		PipelineDescriptor(const PipelineDescriptor&) = default;
		PipelineDescriptor(PipelineDescriptor&&) = default;
		PipelineDescriptor& operator = (const PipelineDescriptor&) = default;
		PipelineDescriptor& operator = (PipelineDescriptor&&) = default;

		rf::ShaderModuleId m_VertexShaderId;
		rf::ShaderModuleId m_FragmentShaderId;
		rf::BlendStateId m_BlendStateId;
		rf::DepthStateId m_DepthStateId;
		rf::RasterizationStateId m_RasterizationStateId;
		rf::PipelineLayoutId m_PipelineLayoutId;
		rf::VertexDescriptionId m_VertexDescriptionId;
		rf::PassId m_RenderPassId;

		rf::EPrimitiveTopology m_PrimitiveTopology = EPrimitiveTopology::Triangles;
	};
}