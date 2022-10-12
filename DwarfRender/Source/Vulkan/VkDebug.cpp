#include "VkDebug.h"

#if DF_USE_DEBUG_MARKERS
void vk::SetDebugName(VkDevice device, VkInstance handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_INSTANCE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkPhysicalDevice handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_PHYSICAL_DEVICE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkDevice handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_DEVICE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkQueue handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_QUEUE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkSemaphore handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_SEMAPHORE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkCommandBuffer handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_COMMAND_BUFFER;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkFence handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_FENCE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkDeviceMemory handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkBuffer handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_BUFFER;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkImage handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_IMAGE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkEvent handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_EVENT;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkQueryPool handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_QUERY_POOL;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkBufferView handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_BUFFER_VIEW;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkImageView handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_IMAGE_VIEW;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkShaderModule handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_SHADER_MODULE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkPipelineCache handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_PIPELINE_CACHE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkPipelineLayout handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_PIPELINE_LAYOUT;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkRenderPass handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_RENDER_PASS;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkPipeline handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_PIPELINE;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkDescriptorSetLayout handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkSampler handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_SAMPLER;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkDescriptorPool handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkDescriptorSet handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_DESCRIPTOR_SET;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkFramebuffer handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_FRAMEBUFFER;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}

void vk::SetDebugName(VkDevice device, VkCommandPool handle, const char* name) {
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectType = VK_OBJECT_TYPE_COMMAND_POOL;
	info.objectHandle = (uint64_t)handle;
	info.pObjectName = name;

	vk::API::SetDebugUtilsObjectNameEXT(device, &info);
}
#endif