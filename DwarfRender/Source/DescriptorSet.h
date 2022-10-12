#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkDescriptorSet.h"
#endif

#include <DwarfRender/ObjectId.h>

namespace rf {
	struct DescriptorSet {
		void Begin() const;
		void End() const;
		void SetStorageBuffer(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size, uint32 offset = 0) const;
		void SetUniformBuffer(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size, uint32 offset = 0) const;
		void SetUniformBufferDynamic(uint32 binding, uint32 arrayIdx, rf::BufferId buffer, uint32 size) const;
		void SetCombinedImageSampler(uint32 binding, uint32 arrayIdx, rf::TextureId texture, rf::SamplerId sampler) const;

		api::DescriptorSet m_APIData;
		uint32 m_Id;
	};
}