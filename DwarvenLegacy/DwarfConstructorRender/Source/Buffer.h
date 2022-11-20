#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkBuffer.h"
#endif

#include <DwarfConstructorRender/Types.h>

namespace rf {
	struct Buffer {
		rf::api::Buffer m_APIData;

		uint32 m_Size;
		uint32 m_Stride;
		uint32 m_UpdateSize;
		BufferUsageFlags m_UsageFlags;
		EBufferAccessType m_AccessType;
		bool m_IsStatic;

		uint8* m_Data;
	};
}