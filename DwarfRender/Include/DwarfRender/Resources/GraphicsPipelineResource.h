#pragma once

#include <DwarfResources/Resource.h>

#include <DwarfRender/ObjectId.h>

namespace rf {
	class GraphicsPipelineResource : public df::IResource {
	public:
		GraphicsPipelineId m_GraphicsPipeline = nullptr;
	};
}