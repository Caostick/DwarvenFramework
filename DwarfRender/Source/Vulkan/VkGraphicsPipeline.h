#pragma once

#include "VkAPI.h"
#include "VkVertexDescription.h"

#include <DwarvenCore/Vector.h>

namespace rf {
	namespace api {
		struct GraphicsPipeline {
			struct PipelineInstance {
				VkPipeline m_Pipeline = 0;
				VkRenderPass m_RenderPass = 0;
			};

			//VkShaderModule m_VSModule = 0;
			//VkShaderModule m_FSModule = 0;
			VkPipelineLayout m_Layout = 0;
			df::Vector<PipelineInstance> m_Instances;

			vk::VertexDescription m_VertexDescription;
		};
	}
}