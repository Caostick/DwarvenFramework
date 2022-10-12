#pragma once

#include "Vulkan/VkVertexDescription.h"

namespace rf {
	struct VertexDescription {
		VertexDescription() = default;
		VertexDescription(const VertexDescription&) = default;
		VertexDescription(VertexDescription&&) = default;
		VertexDescription& operator = (const VertexDescription&) = default;
		VertexDescription& operator = (VertexDescription&&) = default;

		bool operator == (const VertexDescription& other) const;
		bool operator != (const VertexDescription& other) const;

		vk::VertexDescription m_APIData;
	};
}