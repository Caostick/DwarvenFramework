#pragma once

#include "VkDefinitions.h"

#include <vulkan/vulkan.h>

namespace vk {
    class API {
    public:
        static bool Load();
        static bool LoadInstanceFunctions(VkInstance instance);
        static bool LoadDeviceFunctions(VkDevice device);

        static void* VulkanLib;

#define VK_FUNCTION(func) static PFN_vk##func func

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
        VK_FUNCTION(CmdBeginRenderPass);
        VK_FUNCTION(CmdBindDescriptorSets);
        VK_FUNCTION(CmdBindIndexBuffer);
        VK_FUNCTION(CmdBindPipeline);
        VK_FUNCTION(CmdDrawMeshTasksNV);
        VK_FUNCTION(CmdDrawMeshTasksIndirectNV);
        VK_FUNCTION(CmdDrawMeshTasksIndirectCountNV);
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
    };
}