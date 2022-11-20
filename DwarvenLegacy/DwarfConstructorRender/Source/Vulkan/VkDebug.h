#pragma once

#include "VkAPI.h"

namespace vk {
#if DF_USE_DEBUG_MARKERS
	void SetDebugName(VkDevice device, VkInstance handle, const char* name);
	void SetDebugName(VkDevice device, VkPhysicalDevice handle, const char* name);
	void SetDebugName(VkDevice device, VkDevice handle, const char* name);
	void SetDebugName(VkDevice device, VkQueue handle, const char* name);
	void SetDebugName(VkDevice device, VkSemaphore handle, const char* name);
	void SetDebugName(VkDevice device, VkCommandBuffer handle, const char* name);
	void SetDebugName(VkDevice device, VkFence handle, const char* name);
	void SetDebugName(VkDevice device, VkDeviceMemory handle, const char* name);
	void SetDebugName(VkDevice device, VkBuffer handle, const char* name);
	void SetDebugName(VkDevice device, VkImage handle, const char* name);
	void SetDebugName(VkDevice device, VkEvent handle, const char* name);
	void SetDebugName(VkDevice device, VkQueryPool handle, const char* name);
	void SetDebugName(VkDevice device, VkBufferView handle, const char* name);
	void SetDebugName(VkDevice device, VkImageView handle, const char* name);
	void SetDebugName(VkDevice device, VkShaderModule handle, const char* name);
	void SetDebugName(VkDevice device, VkPipelineCache handle, const char* name);
	void SetDebugName(VkDevice device, VkPipelineLayout handle, const char* name);
	void SetDebugName(VkDevice device, VkRenderPass handle, const char* name);
	void SetDebugName(VkDevice device, VkPipeline handle, const char* name);
	void SetDebugName(VkDevice device, VkDescriptorSetLayout handle, const char* name);
	void SetDebugName(VkDevice device, VkSampler handle, const char* name);
	void SetDebugName(VkDevice device, VkDescriptorPool handle, const char* name);
	void SetDebugName(VkDevice device, VkDescriptorSet handle, const char* name);
	void SetDebugName(VkDevice device, VkFramebuffer handle, const char* name);
	void SetDebugName(VkDevice device, VkCommandPool handle, const char* name);
	//void SetDebugName(VkDevice device, VkSamplerYcbcrConversion handle, const char* name);
	//void SetDebugName(VkDevice device, VkDescriptorUpdateTemplate handle, const char* name);
	//void SetDebugName(VkDevice device, VkSurfaceKHR handle, const char* name);
	//void SetDebugName(VkDevice device, VkSwapchainKHR handle, const char* name);
	//void SetDebugName(VkDevice device, VkDisplayKHR handle, const char* name);
	//void SetDebugName(VkDevice device, VkDisplayModeKHR handle, const char* name);
	//void SetDebugName(VkDevice device, VkDebugReportCallbackEXT handle, const char* name);
	//void SetDebugName(VkDevice device, VkDebugUtilsMessengerEXT handle, const char* name);
	//void SetDebugName(VkDevice device, VkAccelerationStructureKHR handle, const char* name);
	//void SetDebugName(VkDevice device, VkValidationCacheEXT handle, const char* name);
	//void SetDebugName(VkDevice device, VkPerformanceConfigurationINTEL handle, const char* name);
	//void SetDebugName(VkDevice device, VkIndirectCommandsLayoutNV handle, const char* name);
	//void SetDebugName(VkDevice device, VkPrivateDataSlotEXT handle, const char* name);
#endif
}

#if DF_USE_DEBUG_MARKERS
#include <DwarvenCore/StringView.h>

template <typename T, typename ...TArgs>
void DFVkDebugName(VkDevice device, T handle, const df::StringView& fmt, TArgs... args) {
	char debugNameBuffer[256];
	sprintf_s(debugNameBuffer, fmt.data(), args...);
	vk::SetDebugName(device, handle, debugNameBuffer);
}
#else
#define DFVkDebugName
#endif