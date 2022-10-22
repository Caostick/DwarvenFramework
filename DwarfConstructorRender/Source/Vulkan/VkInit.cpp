#include "VkInit.h"
#include "VkAPI.h"

#include <DwarvenCore/Assert.h>

namespace /*anonymous*/ {
    constexpr const uint32_t NumInstanceExtensions = 3
#if DF_USE_DEBUG_MARKERS
		+ 1
#endif
        ;
    constexpr const char* InstanceExtensions[NumInstanceExtensions] = {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
#if VK_DEBUG
		"VK_EXT_debug_utils",
#endif
#if DF_USE_DEBUG_MARKERS
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
    };

    constexpr const uint32_t NumDeviceExtensions = 1
#if DF_USE_DEBUG_MARKERS
        + 1
#endif
        ;

    constexpr const char* DeviceExtensions[NumDeviceExtensions] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if DF_USE_DEBUG_MARKERS
		VK_EXT_DEBUG_MARKER_EXTENSION_NAME
#endif
    };

    constexpr const uint32_t NumValidationLayers = 1;
    constexpr const char* ValidationLayers[NumValidationLayers] = {
        //"VK_LAYER_LUNARG_standard_validation",
        "VK_LAYER_KHRONOS_validation",
        //"VK_LAYER_LUNARG_core_validation",
        //"VK_LAYER_RENDERDOC_Capture"
    };
}

#if VK_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT /*messageSeverity*/,
    VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
    [[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* /*pUserData*/) {

    DFAssert(false, pCallbackData->pMessage);
    return VK_FALSE;
}

namespace /*anonymous*/ {
    VkDebugUtilsMessengerEXT DebugMessenger;
    VkDebugReportCallbackEXT DebugReportCallback;


    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        return vk::API::CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        vk::API::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, pAllocator);
    }
}
#endif



uint32_t vk::GetNumRequiredInstanceExtensions() {
    return NumInstanceExtensions;
}

const char** vk::GetRequiredInstanceExtensions() {
    return (const char**)InstanceExtensions;
}

uint32_t vk::GetNumRequiredDeviceExtensions() {
    return NumDeviceExtensions;
}

const char** vk::GetRequiredDeviceExtensions() {
    return (const char**)DeviceExtensions;
}

uint32_t vk::GetNumRequiredValidationLayers() {
    return NumValidationLayers;
}

const char** vk::GetRequiredValidationLayers() {
    return (const char**)ValidationLayers;
}

bool vk::CreateDebugCallback([[maybe_unused]] VkInstance instance) {
#if VK_DEBUG
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pUserData = nullptr;
    createInfo.pNext = nullptr;
    createInfo.pfnUserCallback = VulkanDebugCallback;

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS) {
        DFAssert(false, "Failed to set up debug messenger!");

        return false;
    }
#endif

    return true;
}

void vk::DestroyDebugCallback([[maybe_unused]] VkInstance instance) {
#if VK_DEBUG
    DestroyDebugUtilsMessengerEXT(instance, DebugMessenger, nullptr);
#endif
}
