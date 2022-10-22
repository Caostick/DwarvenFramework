#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vk {
	auto Allocator() -> const VkAllocationCallbacks*;
	auto GetErrorString(VkResult error) -> const char*;
}