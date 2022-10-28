#pragma once

#include "VkAPI.h"

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Window;
}

namespace vk {
	class RenderCore;
	class CommandBuffer;
	class RenderPass;
	class Texture;
	class ParameterSet;
	class GraphicsPipeline;
}

namespace vk {
	class Presentation {
	public:
		Presentation();

		bool CreateSurface(VkInstance instance, const df::Window& window);
		void DestroySurface(VkInstance instance);

		bool CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
		void DestroySwapchain(VkDevice device);

		auto GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex)->VkBool32;
		auto AquireNextImage(VkDevice device, VkSemaphore semaphore)->VkResult;
		auto Present(VkSemaphore semaphore, VkQueue presentQueue)->VkResult;

		bool Load(vk::RenderCore& renderCore);
		void Unload(vk::RenderCore& renderCore);

		bool RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
		bool RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);

		void PresentTexture(vk::CommandBuffer& rcb, vk::Texture* texture);

	private:
		VkSurfaceKHR m_VkSurface;
		VkSwapchainKHR m_VkSwapchain;
		VkExtent2D m_VkExtent;
		VkFormat m_VkFormat;
		bool m_VSyncEnabled;

		uint32 m_MinImagesCount;
		uint32 m_ImagesCount;
		uint32 m_AvailableImageIndex;

		df::Vector<VkImage> m_Images;

		df::Vector<VkImageView> m_ImageViews;
		df::Vector<vk::RenderPass*> m_RenderPasses;
		vk::GraphicsPipeline* m_Pipeline;
		vk::ParameterSet* m_ParametrSet;
	};
}