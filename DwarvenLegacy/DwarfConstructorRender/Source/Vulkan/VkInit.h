#pragma once

#include <vulkan/vulkan.h>

#define VK_DEBUG 1

#if VK_DEBUG
#define SET_DEBUG_NAME(device, x, name) SetDebugName(device, x, name)
#else
#define SET_DEBUG_NAME
#endif

namespace vk {
    uint32_t GetNumRequiredInstanceExtensions();
    const char** GetRequiredInstanceExtensions();

    uint32_t GetNumRequiredDeviceExtensions();
    const char** GetRequiredDeviceExtensions();

    uint32_t GetNumRequiredValidationLayers();
    const char** GetRequiredValidationLayers();

    bool CreateDebugCallback(VkInstance instance);
    void DestroyDebugCallback(VkInstance instance);
}