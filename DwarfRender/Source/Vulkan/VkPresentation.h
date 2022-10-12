#pragma once

#include "VkAPI.h"

#include "../Pass.h"
#include "../Framebuffer.h"

#include <DwarfRender/ObjectId.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Window;
}

namespace rf {
	class RenderCore;
	class CommandBuffer;
}

namespace rf {
	namespace api {
		class Presentation {
		public:
			Presentation();

			bool CreateSurface(VkInstance instance, df::Window* window);
			void DestroySurface(VkInstance instance);

			bool CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
			void DestroySwapchain(VkDevice device);

			auto GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex)->VkBool32;
			auto AquireNextImage(VkDevice device, VkSemaphore semaphore) -> VkResult;
			auto Present(VkSemaphore semaphore, VkQueue presentQueue) -> VkResult;

			bool Load(rf::RenderCore& renderCore);
			void Unload(rf::RenderCore& renderCore);

			bool RecreateSwapchain(rf::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);
			bool RecreateSwapchain(rf::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex);

			void PresentTexture(rf::CommandBuffer& rcb, rf::TextureId textureId);

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
			df::Vector<rf::DescriptorSetId> m_DescriptorSets;

			rf::Pass m_RenderPass;
			df::Vector<rf::Framebuffer> m_Framebuffers;

			rf::DescriptorSetLayoutId m_ShaderDescriptorSetLayoutId;
			rf::PipelineId m_PipelineId;
			rf::PipelineLayoutId m_PipelineLayoutId;
			rf::ShaderModuleId m_VertexShaderId;
			rf::ShaderModuleId m_FragmentShaderId;
		};
	}
}