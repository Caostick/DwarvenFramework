#pragma once

#include "VkAPI.h"
#include "VkVertexDescription.h"

#include <DwarvenCore/Vector.h>

namespace rf {
	namespace api {
		struct GraphicsPipeline {
			VkShaderModule m_VSModule = 0;
			VkShaderModule m_FSModule = 0;
			VkPipelineLayout m_Layout = 0;
			VkPipeline m_Pipeline = 0;

			vk::VertexDescription m_VertexDescription;
		};
	}
}