#pragma once

#include "VkAPI.h"

namespace vk {
	class RenderCore;
	class ParameterSet;
	class GraphicsPipeline;
}

namespace vk {
	class PresentationPipeline {
	public:
		bool Init(vk::RenderCore& renderCore);
		void Release(vk::RenderCore& renderCore);

		auto GetPipeline() const->vk::GraphicsPipeline*;
		auto GetParameterSet() const->vk::ParameterSet*;

	private:
		vk::GraphicsPipeline* m_Pipeline;
		vk::ParameterSet* m_ParametrSet;
	};
}