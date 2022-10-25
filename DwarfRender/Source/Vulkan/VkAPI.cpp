#include "VkAPI.h"

#include <DwarvenCore/Assert.h>

#include <windows.h>
#undef CreateSemaphore


#define LOAD_GLOBAL_FUNCTION(func) \
    func = (PFN_vk##func)GetProcAddress((HMODULE)VulkanLib, "vk"#func); \
    if(!func) { \
        DFAssert(false, "Can't load function "#func); \
        return false; \
    }


#define LOAD_INSTANCE_FUNCTION(func) \
    func = (PFN_vk##func)GetInstanceProcAddr(instance, "vk"#func); \
    if(!func) { \
        DFAssert(false, "Can't load function "#func); \
        return false; \
    }

#define LOAD_DEVICE_FUNCTION(func) \
    func = (PFN_vk##func)GetDeviceProcAddr(device, "vk"#func); \
    if(!func) { \
        DFAssert(false, "Can't load function "#func); \
        return false; \
    }




bool vk::API::Load() {
    VulkanLib = LoadLibrary("vulkan-1.dll");
    if (!VulkanLib) {
        return false;
    }

    LOAD_GLOBAL_FUNCTION(CreateInstance);
    LOAD_GLOBAL_FUNCTION(DestroyInstance);
    LOAD_GLOBAL_FUNCTION(GetInstanceProcAddr);

    return true;
}

bool vk::API::LoadInstanceFunctions(VkInstance instance) {
    LOAD_INSTANCE_FUNCTION(EnumerateDeviceLayerProperties);
    LOAD_INSTANCE_FUNCTION(CreateDebugUtilsMessengerEXT);
    LOAD_INSTANCE_FUNCTION(DestroyDebugUtilsMessengerEXT);
    LOAD_INSTANCE_FUNCTION(GetDeviceProcAddr);
    LOAD_INSTANCE_FUNCTION(EnumeratePhysicalDevices);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceProperties);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceFeatures);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceQueueFamilyProperties);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceSurfaceSupportKHR);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceSurfaceCapabilitiesKHR);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceSurfaceFormatsKHR);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceSurfacePresentModesKHR);
    LOAD_INSTANCE_FUNCTION(GetPhysicalDeviceMemoryProperties);
    LOAD_INSTANCE_FUNCTION(CreateDevice);
    LOAD_INSTANCE_FUNCTION(DestroySurfaceKHR);
#if DF_USE_DEBUG_MARKERS
    LOAD_INSTANCE_FUNCTION(SetDebugUtilsObjectNameEXT);
    LOAD_INSTANCE_FUNCTION(SetDebugUtilsObjectTagEXT);
#endif

    return true;
}

bool vk::API::LoadDeviceFunctions(VkDevice device) {
    
    LOAD_DEVICE_FUNCTION(AcquireNextImageKHR);
    LOAD_DEVICE_FUNCTION(AllocateCommandBuffers);
    LOAD_DEVICE_FUNCTION(AllocateDescriptorSets);
    LOAD_DEVICE_FUNCTION(AllocateMemory);
    LOAD_DEVICE_FUNCTION(BeginCommandBuffer);
    LOAD_DEVICE_FUNCTION(BindBufferMemory);
    LOAD_DEVICE_FUNCTION(BindImageMemory);
    LOAD_DEVICE_FUNCTION(CmdBlitImage);
    LOAD_DEVICE_FUNCTION(CmdBeginRenderPass);
    LOAD_DEVICE_FUNCTION(CmdBindDescriptorSets);
    LOAD_DEVICE_FUNCTION(CmdBindIndexBuffer);
    LOAD_DEVICE_FUNCTION(CmdBindPipeline);
    LOAD_DEVICE_FUNCTION(CmdBindVertexBuffers);
    LOAD_DEVICE_FUNCTION(CmdCopyBuffer);
    LOAD_DEVICE_FUNCTION(CmdCopyBufferToImage);
    LOAD_DEVICE_FUNCTION(CmdDraw);
    LOAD_DEVICE_FUNCTION(CmdDrawIndexed);
    LOAD_DEVICE_FUNCTION(CmdEndRenderPass);
    LOAD_DEVICE_FUNCTION(CmdSetLineWidth);
    LOAD_DEVICE_FUNCTION(CmdPipelineBarrier);
    LOAD_DEVICE_FUNCTION(CmdSetScissor);
    LOAD_DEVICE_FUNCTION(CmdSetViewport);
#if DF_USE_DEBUG_MARKERS
    LOAD_DEVICE_FUNCTION(DebugMarkerSetObjectTagEXT);
    LOAD_DEVICE_FUNCTION(DebugMarkerSetObjectNameEXT);
    LOAD_DEVICE_FUNCTION(CmdDebugMarkerBeginEXT);
    LOAD_DEVICE_FUNCTION(CmdDebugMarkerEndEXT);
    LOAD_DEVICE_FUNCTION(CmdDebugMarkerInsertEXT);
#endif
    LOAD_DEVICE_FUNCTION(CreateBuffer);
    LOAD_DEVICE_FUNCTION(CreateBufferView);
    LOAD_DEVICE_FUNCTION(CreateCommandPool);
    LOAD_DEVICE_FUNCTION(CreateDescriptorPool);
    LOAD_DEVICE_FUNCTION(CreateDescriptorSetLayout);
    LOAD_DEVICE_FUNCTION(CreateFence);
    LOAD_DEVICE_FUNCTION(CreateFramebuffer);
    LOAD_DEVICE_FUNCTION(CreateGraphicsPipelines);
    LOAD_DEVICE_FUNCTION(CreateImage);
    LOAD_DEVICE_FUNCTION(CreateImageView);
    LOAD_DEVICE_FUNCTION(CreatePipelineLayout);
    LOAD_DEVICE_FUNCTION(CreateRenderPass);
    LOAD_DEVICE_FUNCTION(CreateSampler);
    LOAD_DEVICE_FUNCTION(CreateSemaphore);
    LOAD_DEVICE_FUNCTION(CreateShaderModule);
    LOAD_DEVICE_FUNCTION(CreateSwapchainKHR);
    LOAD_DEVICE_FUNCTION(DestroyBuffer);
    LOAD_DEVICE_FUNCTION(DestroyBufferView);
    LOAD_DEVICE_FUNCTION(DestroyCommandPool);
    LOAD_DEVICE_FUNCTION(DestroyDescriptorPool);
    LOAD_DEVICE_FUNCTION(DestroyDescriptorSetLayout);
    LOAD_DEVICE_FUNCTION(DestroyDevice);
    LOAD_DEVICE_FUNCTION(DestroyFence);
    LOAD_DEVICE_FUNCTION(DestroyFramebuffer);
    LOAD_DEVICE_FUNCTION(DestroyImage);
    LOAD_DEVICE_FUNCTION(DestroyImageView);
    LOAD_DEVICE_FUNCTION(DestroyPipeline);
    LOAD_DEVICE_FUNCTION(DestroyPipelineLayout);
    LOAD_DEVICE_FUNCTION(DestroyRenderPass);
    LOAD_DEVICE_FUNCTION(DestroySampler);
    LOAD_DEVICE_FUNCTION(DestroySemaphore);
    LOAD_DEVICE_FUNCTION(DestroyShaderModule);
    LOAD_DEVICE_FUNCTION(DestroySwapchainKHR);
    LOAD_DEVICE_FUNCTION(DeviceWaitIdle);
    LOAD_DEVICE_FUNCTION(EndCommandBuffer);
    LOAD_DEVICE_FUNCTION(FreeCommandBuffers);
    LOAD_DEVICE_FUNCTION(FreeDescriptorSets);
    LOAD_DEVICE_FUNCTION(FreeMemory);
    LOAD_DEVICE_FUNCTION(GetBufferMemoryRequirements);
    LOAD_DEVICE_FUNCTION(GetDeviceQueue);
    LOAD_DEVICE_FUNCTION(GetImageMemoryRequirements);
    LOAD_DEVICE_FUNCTION(GetSwapchainImagesKHR);
    LOAD_DEVICE_FUNCTION(MapMemory);
    LOAD_DEVICE_FUNCTION(QueuePresentKHR);
    LOAD_DEVICE_FUNCTION(QueueSubmit);
    LOAD_DEVICE_FUNCTION(QueueWaitIdle);
    LOAD_DEVICE_FUNCTION(ResetFences);
    LOAD_DEVICE_FUNCTION(UnmapMemory);
    LOAD_DEVICE_FUNCTION(UpdateDescriptorSets);
    LOAD_DEVICE_FUNCTION(WaitForFences);

    return true;
}

void* vk::API::VulkanLib = nullptr;

#define VK_FUNCTION(func) PFN_vk##func vk::API::func = nullptr;

// Global
VK_FUNCTION(CreateInstance);
VK_FUNCTION(DestroyInstance);
VK_FUNCTION(GetInstanceProcAddr);

// Instance
VK_FUNCTION(EnumerateDeviceLayerProperties);
VK_FUNCTION(CreateDebugUtilsMessengerEXT);
VK_FUNCTION(CreateDevice);
VK_FUNCTION(DestroyDebugUtilsMessengerEXT);
VK_FUNCTION(DestroySurfaceKHR);
VK_FUNCTION(EnumeratePhysicalDevices);
VK_FUNCTION(GetDeviceProcAddr);
VK_FUNCTION(GetPhysicalDeviceFeatures);
VK_FUNCTION(GetPhysicalDeviceMemoryProperties);
VK_FUNCTION(GetPhysicalDeviceProperties);
VK_FUNCTION(GetPhysicalDeviceQueueFamilyProperties);
VK_FUNCTION(GetPhysicalDeviceSurfaceCapabilitiesKHR);
VK_FUNCTION(GetPhysicalDeviceSurfaceFormatsKHR);
VK_FUNCTION(GetPhysicalDeviceSurfacePresentModesKHR);
VK_FUNCTION(GetPhysicalDeviceSurfaceSupportKHR);
#if DF_USE_DEBUG_MARKERS
VK_FUNCTION(SetDebugUtilsObjectNameEXT);
VK_FUNCTION(SetDebugUtilsObjectTagEXT);
#endif

// Device
VK_FUNCTION(AcquireNextImageKHR);
VK_FUNCTION(AllocateCommandBuffers);
VK_FUNCTION(AllocateDescriptorSets);
VK_FUNCTION(AllocateMemory);
VK_FUNCTION(BeginCommandBuffer);
VK_FUNCTION(BindBufferMemory);
VK_FUNCTION(BindImageMemory);
VK_FUNCTION(CmdBlitImage);
VK_FUNCTION(CmdBeginRenderPass);
VK_FUNCTION(CmdBindDescriptorSets);
VK_FUNCTION(CmdBindIndexBuffer);
VK_FUNCTION(CmdBindPipeline);
VK_FUNCTION(CmdBindVertexBuffers);
VK_FUNCTION(CmdCopyBuffer);
VK_FUNCTION(CmdCopyBufferToImage);
VK_FUNCTION(CmdDraw);
VK_FUNCTION(CmdDrawIndexed);
VK_FUNCTION(CmdEndRenderPass);
VK_FUNCTION(CmdSetLineWidth);
VK_FUNCTION(CmdPipelineBarrier);
VK_FUNCTION(CmdSetScissor);
VK_FUNCTION(CmdSetViewport);
#if DF_USE_DEBUG_MARKERS
VK_FUNCTION(DebugMarkerSetObjectTagEXT);
VK_FUNCTION(DebugMarkerSetObjectNameEXT);
VK_FUNCTION(CmdDebugMarkerBeginEXT);
VK_FUNCTION(CmdDebugMarkerEndEXT);
VK_FUNCTION(CmdDebugMarkerInsertEXT);
#endif
VK_FUNCTION(CreateBuffer);
VK_FUNCTION(CreateBufferView);
VK_FUNCTION(CreateCommandPool);
VK_FUNCTION(CreateDescriptorPool);
VK_FUNCTION(CreateDescriptorSetLayout);
VK_FUNCTION(CreateFence);
VK_FUNCTION(CreateFramebuffer);
VK_FUNCTION(CreateGraphicsPipelines);
VK_FUNCTION(CreateImage);
VK_FUNCTION(CreateImageView);
VK_FUNCTION(CreatePipelineLayout);
VK_FUNCTION(CreateRenderPass);
VK_FUNCTION(CreateSampler);
VK_FUNCTION(CreateSemaphore);
VK_FUNCTION(CreateShaderModule);
VK_FUNCTION(CreateSwapchainKHR);
VK_FUNCTION(DestroyBuffer);
VK_FUNCTION(DestroyBufferView);
VK_FUNCTION(DestroyCommandPool);
VK_FUNCTION(DestroyDescriptorPool);
VK_FUNCTION(DestroyDescriptorSetLayout);
VK_FUNCTION(DestroyDevice);
VK_FUNCTION(DestroyFence);
VK_FUNCTION(DestroyFramebuffer);
VK_FUNCTION(DestroyImage);
VK_FUNCTION(DestroyImageView);
VK_FUNCTION(DestroyPipeline);
VK_FUNCTION(DestroyPipelineLayout);
VK_FUNCTION(DestroyRenderPass);
VK_FUNCTION(DestroySampler);
VK_FUNCTION(DestroySemaphore);
VK_FUNCTION(DestroyShaderModule);
VK_FUNCTION(DestroySwapchainKHR);
VK_FUNCTION(DeviceWaitIdle);
VK_FUNCTION(EndCommandBuffer);
VK_FUNCTION(FreeCommandBuffers);
VK_FUNCTION(FreeDescriptorSets);
VK_FUNCTION(FreeMemory);
VK_FUNCTION(GetBufferMemoryRequirements);
VK_FUNCTION(GetDeviceQueue);
VK_FUNCTION(GetImageMemoryRequirements);
VK_FUNCTION(GetSwapchainImagesKHR);
VK_FUNCTION(MapMemory);
VK_FUNCTION(QueuePresentKHR);
VK_FUNCTION(QueueSubmit);
VK_FUNCTION(QueueWaitIdle);
VK_FUNCTION(ResetFences);
VK_FUNCTION(UnmapMemory);
VK_FUNCTION(UpdateDescriptorSets);
VK_FUNCTION(WaitForFences);

#undef VK_FUNCTION