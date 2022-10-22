#pragma once

#include <DwarvenCore/Types.h>

#define VK_DEBUG 1
#define DF_USE_DEBUG_MARKERS 1

namespace vk {
	namespace Constants {
		constexpr uint32 MaxFramesInFlight = 2;
		constexpr uint32 MaxQueueFamilyIndices = 4;
		constexpr uint32 MaxAttachements = 8;
		constexpr uint32 MaxLayoutBindings = 16;
		constexpr uint32 MaxVertexDescriptions = 2;
		constexpr uint32 NumShaderTypes = 6;
		constexpr uint32 MaxFramebufferAttachements = 8;
		constexpr uint32 DepthAttachementIndex = 8;
		constexpr uint32 MaxBufferBindings = 16;
		constexpr uint32 DepthFormatCount = 5;
		constexpr uint32 MaxDynamicPipelineStates = 9;
		constexpr uint32 MaxDescriptorCount = 16;
	}
}