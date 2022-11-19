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
	class PresentationPipeline;
}

namespace vk {
	class Presentation {
	public:
		Presentation(const df::Window& window, const vk::PresentationPipeline& pipeline);

		auto GetWindow() const -> const df::Window&;

		bool IsValid() const;

		bool CreateSurface(VkInstance instance);
		void DestroySurface(vk::RenderCore& renderCore);

		bool CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
		void DestroySwapchain(vk::RenderCore& renderCore);
		auto GetSwapchain() const->VkSwapchainKHR;
		auto GeiAvailableImageIndex() const->uint32;

		auto GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex)->VkBool32;
		auto AquireNextImage(VkDevice device)->VkResult;
		auto GetImageAvailableSemaphore()const->VkSemaphore;
		auto Present(VkSemaphore semaphore, VkQueue presentQueue)->VkResult;

		bool Load(vk::RenderCore& renderCore);
		void Unload(vk::RenderCore& renderCore);

		bool RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
		bool RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);

		void SetPresentTexture(vk::Texture* texture);
		void PresentTexture(vk::CommandBuffer& rcb);

	private:
		VkSurfaceKHR m_VkSurface;
		VkSwapchainKHR m_VkSwapchain;
		VkExtent2D m_VkExtent;
		VkFormat m_VkFormat;
		bool m_VSyncEnabled;

		uint32 m_MinImagesCount;
		uint32 m_ImagesCount;
		uint32 m_AvailableImageIndex;

		const df::Window& m_Window;
		const vk::PresentationPipeline& m_Pipeline;

		df::Vector<VkImage> m_Images;
		df::Vector<VkSemaphore> m_ImageAvailableSemaphores;
		uint32 m_CurrentSemaphoreIndex;

		df::Vector<VkImageView> m_ImageViews;
		df::Vector<vk::RenderPass*> m_RenderPasses;
		vk::ParameterSet* m_ParametrSet;
		vk::Texture* m_PresentTexture;
	};
}