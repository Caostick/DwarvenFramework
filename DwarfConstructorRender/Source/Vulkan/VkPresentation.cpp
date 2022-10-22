#include "VkPresentation.h"
#include "VkAllocator.h"
#include "VkHelper.h"
#include "VkDebug.h"

#include "../Descriptor.h"
#include "../RenderCore.h"

#include "Generated/Presentation.generated.h"

#include <DwarfWindow/Window.h>

#include <DwarfConstructorRender/GlobalObjects.h>

#include <DwarvenCore/Assert.h>

#ifdef GLFW_WINDOW_IMPLEMENTATION
#include <glfw/glfw3.h>
#endif

namespace {
	const char* g_StageDebugName = "Presentation";
}

namespace /*anonymous*/ {
	struct SwapChainSupportDetails {
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const {
			if (FormatCount == 1 && Formats[0].format == VK_FORMAT_UNDEFINED) {
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : Formats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return Formats[0];
		}

		VkPresentModeKHR ChooseSwapPresentMode() const {
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& availablePresentMode : PresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				} else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					bestMode = availablePresentMode;
				}
			}

			return bestMode;
		}

		VkExtent2D ChooseSwapExtent(const VkExtent2D& requiredExtend) const {
			if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return Capabilities.currentExtent;
			} else {
				VkExtent2D actualExtend;
				actualExtend.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, requiredExtend.width));
				actualExtend.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, requiredExtend.height));

				return actualExtend;
			}
		}

		VkSurfaceCapabilitiesKHR            Capabilities;
		uint32_t                            FormatCount;
		VkSurfaceFormatKHR                  Formats[16];
		uint32_t                            PresentModeCount;
		VkPresentModeKHR                    PresentModes[16];
	};
}



rf::api::Presentation::Presentation()
	:m_VkSurface(VK_NULL_HANDLE)
	, m_VkSwapchain(VK_NULL_HANDLE)
	, m_VkExtent({ 0,0 })
	, m_VkFormat(VK_FORMAT_UNDEFINED)
	, m_MinImagesCount(0)
	, m_ImagesCount(0)
	, m_AvailableImageIndex(0) {}

bool rf::api::Presentation::CreateSurface(VkInstance instance, df::Window* window) {
	DFAssert(window, "Window is null!");
	DFAssert(m_VkSurface == VK_NULL_HANDLE, "Surface already created!");

#ifdef GLFW_WINDOW_IMPLEMENTATION
	if (glfwCreateWindowSurface(instance, window->GetPtr(), vk::Allocator(), &m_VkSurface) != VK_SUCCESS) {
		DFAssert(false, "Can't create Surface!");

		return false;
	}
#else
	static_assert(false, "No window API selected!");
#endif

	return true;
}

void rf::api::Presentation::DestroySurface(VkInstance instance) {
	DFAssert(m_VkSurface != VK_NULL_HANDLE, "Surface not created!");

	vk::API::DestroySurfaceKHR(instance, m_VkSurface, vk::Allocator());
	m_VkSurface = VK_NULL_HANDLE;
}

bool rf::api::Presentation::CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	DFAssert(m_VkSurface != VK_NULL_HANDLE, "Surface wasn't created!");
	DFAssert(m_VkSwapchain == VK_NULL_HANDLE, "Swapchain already created!");

	m_VSyncEnabled = vSyncEnabled;

	SwapChainSupportDetails details;
	{
		vk::API::GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_VkSurface, &details.Capabilities);

		vk::API::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_VkSurface, &details.FormatCount, nullptr);

		if (details.FormatCount != 0) {
			vk::API::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_VkSurface, &details.FormatCount, details.Formats);
		}

		vk::API::GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_VkSurface, &details.PresentModeCount, nullptr);

		if (details.PresentModeCount != 0) {
			vk::API::GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_VkSurface, &details.PresentModeCount, details.PresentModes);
		}
	}

	const VkSurfaceFormatKHR surfaceFormat = details.ChooseSwapSurfaceFormat();
	const VkPresentModeKHR presentMode = m_VSyncEnabled ? VK_PRESENT_MODE_FIFO_KHR : details.ChooseSwapPresentMode();

	m_VkExtent = details.ChooseSwapExtent(extent);
	m_VkFormat = surfaceFormat.format;

	m_MinImagesCount = details.Capabilities.minImageCount + 1;
	if (details.Capabilities.maxImageCount > 0 && m_MinImagesCount > details.Capabilities.maxImageCount) {
		m_MinImagesCount = details.Capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = m_VkSurface;
	swapchainCreateInfo.minImageCount = m_MinImagesCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = m_VkExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32 queueFamilyIndices[] = { graphicsFamilyIndex, presentFamilyIndex };

	if (graphicsFamilyIndex != presentFamilyIndex) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	swapchainCreateInfo.preTransform = details.Capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vk::API::CreateSwapchainKHR(device, &swapchainCreateInfo, vk::Allocator(), &m_VkSwapchain) != VK_SUCCESS) {
		DFAssert(false, "Can't create Swapchain!");

		return false;
	}

	vk::API::GetSwapchainImagesKHR(device, m_VkSwapchain, &m_ImagesCount, nullptr);
	m_Images.resize(m_ImagesCount);
	vk::API::GetSwapchainImagesKHR(device, m_VkSwapchain, &m_ImagesCount, m_Images.data());

	// Image Views
	m_ImageViews.resize(m_ImagesCount);
	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		m_ImageViews[i] = vk::CreateImageView(device, m_Images[i], m_VkFormat, 1);
	}

	return true;
}

void rf::api::Presentation::DestroySwapchain(VkDevice device) {
	DFAssert(m_VkSwapchain != VK_NULL_HANDLE, "Swapchain not created!");

	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		vk::DestroyImageView(device, m_ImageViews[i]);
	}
	m_ImageViews.clear();

	m_Images.clear();

	vk::API::DestroySwapchainKHR(device, m_VkSwapchain, vk::Allocator());
	m_VkSwapchain = VK_NULL_HANDLE;
	m_VkExtent = VkExtent2D({ 0, 0 });
	m_MinImagesCount = 0;
	m_ImagesCount = 0;
}

auto rf::api::Presentation::GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex)->VkBool32 {
	VkBool32 presentSupport;
	vk::API::GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_VkSurface, &presentSupport);

	return presentSupport;
}

auto rf::api::Presentation::AquireNextImage(VkDevice device, VkSemaphore semaphore) -> VkResult {
	return vk::API::AcquireNextImageKHR(device, m_VkSwapchain, std::numeric_limits<uint64_t>::max(), semaphore, VK_NULL_HANDLE, &m_AvailableImageIndex);
}

auto rf::api::Presentation::Present(VkSemaphore semaphore, VkQueue presentQueue)->VkResult {
	const VkSemaphore signalSemaphores[] = { semaphore };
	const VkSwapchainKHR swapchains[] = { m_VkSwapchain };

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.pImageIndices = &m_AvailableImageIndex;

	return vk::API::QueuePresentKHR(presentQueue, &presentInfo);
}

bool rf::api::Presentation::Load(rf::RenderCore& renderCore) {
	// Load shaders
	{
		m_VertexShaderId = renderCore.CreateShaderModule("PresentVS", generated::presentation::vsCodeData, generated::presentation::vsCodeLength);
		m_FragmentShaderId = renderCore.CreateShaderModule("PresentFS", generated::presentation::fsCodeData, generated::presentation::fsCodeLength);
	}

	// Create render pass
	{
		VkAttachmentDescription attachementDescription = {};
		attachementDescription.format = m_VkFormat;
		attachementDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachementDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachementDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachementDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachementDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachementDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachementDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		rf::ClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

		renderCore.GetAPIData().InitRenderPass(&m_RenderPass, g_StageDebugName, { attachementDescription }, { clearValue });
	}

	// Create shader descriptor set layout
	{
		rf::DescriptorLayout vsLayout;
		rf::DescriptorLayout fsLayout;
		fsLayout.m_Descriptors[0] = rf::EDescriptorType::CombinedImageSampler;

		m_ShaderDescriptorSetLayoutId = renderCore.CreateDescriptorSetLayout(
			g_StageDebugName,
			vsLayout,
			fsLayout
		);
	}

	// Create pipeline layout
	{
		const df::Vector<rf::DescriptorSetLayoutId> descriptorSetLayouts = {
			m_ShaderDescriptorSetLayoutId
		};

		m_PipelineLayoutId = renderCore.CreatePipelineLayout(g_StageDebugName, descriptorSetLayouts);
	}

	rf::PipelineDescriptor pipelineDescriptor;
	pipelineDescriptor.m_RenderPassId = &m_RenderPass;
	pipelineDescriptor.m_BlendStateId = rf::GlobalObjects::Get(EBlendState::None);
	pipelineDescriptor.m_DepthStateId = rf::GlobalObjects::Get(EDepthStencilState::Always);
	pipelineDescriptor.m_RasterizationStateId = rf::GlobalObjects::Get(ERasterizationState::Fill);
	pipelineDescriptor.m_PipelineLayoutId = m_PipelineLayoutId;
	pipelineDescriptor.m_VertexDescriptionId = renderCore.RequestEmptyVertexDescription();
	pipelineDescriptor.m_VertexShaderId = m_VertexShaderId;
	pipelineDescriptor.m_FragmentShaderId = m_FragmentShaderId;

	m_PipelineId = renderCore.CreatePipelineStateObject(g_StageDebugName, pipelineDescriptor);

	// Framebuffers
	{
		m_Framebuffers.resize(m_ImagesCount);
		for (uint32 i = 0; i < m_ImagesCount; ++i) {
			const df::Vector<VkImageView> rt = { m_ImageViews[i] };
			renderCore.GetAPIData().InitFramebuffer(&m_Framebuffers[i], g_StageDebugName, &m_RenderPass, rt, m_VkExtent.width, m_VkExtent.height);
		}
	}

	// Descriptor sets
	{
		m_DescriptorSets.resize(m_ImagesCount);
		for (auto& ds : m_DescriptorSets) {
			ds = renderCore.CreateDescriptorSet(g_StageDebugName, 0, m_ShaderDescriptorSetLayoutId);
		}
	}

	return true;
}

void rf::api::Presentation::Unload(rf::RenderCore& renderCore) {
	for (auto& fb : m_Framebuffers) {
		renderCore.GetAPIData().ReleaseFramebuffer(&fb);
	}
	m_Framebuffers.clear();

	for (auto& ds : m_DescriptorSets) {
		renderCore.DestroyDescriptorSet(ds);
	}
	m_DescriptorSets.clear();

	renderCore.DestroyPipelineStateObject(m_PipelineId);
	m_PipelineId = nullptr;

	renderCore.DestroyPipelineLayout(m_PipelineLayoutId);
	m_PipelineLayoutId = nullptr;

	renderCore.DestroyDescriptorSetLayout(m_ShaderDescriptorSetLayoutId);
	m_ShaderDescriptorSetLayoutId = nullptr;

	renderCore.GetAPIData().ReleaseRenderPass(&m_RenderPass);

	renderCore.DestroyShaderModule(m_FragmentShaderId);
	renderCore.DestroyShaderModule(m_VertexShaderId);
}

bool rf::api::Presentation::RecreateSwapchain(rf::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	for (auto& fb : m_Framebuffers) {
		renderCore.GetAPIData().ReleaseFramebuffer(&fb);
	}

	DestroySwapchain(device);
	if (!CreateSwapchain(device, physicalDevice, extent, vSyncEnabled, graphicsFamilyIndex, presentFamilyIndex)) {
		return false;
	}

	m_Framebuffers.resize(m_ImagesCount);
	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		const df::Vector<VkImageView> rt = { m_ImageViews[i] };
		renderCore.GetAPIData().InitFramebuffer(&m_Framebuffers[i], g_StageDebugName, &m_RenderPass, rt, m_VkExtent.width, m_VkExtent.height);
	}

	return true;
}

bool rf::api::Presentation::RecreateSwapchain(rf::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	return RecreateSwapchain(renderCore, device, physicalDevice, m_VkExtent, m_VSyncEnabled, graphicsFamilyIndex, presentFamilyIndex);
}

void rf::api::Presentation::PresentTexture(rf::CommandBuffer& rcb, rf::TextureId textureId) {
	DFScopedRenderEvent(rcb, "Present Render Stage");

	const rf::SamplerId samplerId = rf::GlobalObjects::Get(ESamplerState::Nearest);
	const rf::PassId renderPassId = &m_RenderPass;
	const rf::PipelineId pipelineId = m_PipelineId;
	const rf::FramebufferId framebufferId = &m_Framebuffers[m_AvailableImageIndex];
	const rf::DescriptorSetId descriptorSetId = m_DescriptorSets[m_AvailableImageIndex];

	descriptorSetId->Begin();
	descriptorSetId->SetCombinedImageSampler(0, 0, textureId, samplerId);
	descriptorSetId->End();

	rcb.BeginRenderPass(renderPassId, framebufferId);
	rcb.BindPipeline(pipelineId);
	rcb.BindDescriptorSet(m_PipelineLayoutId, descriptorSetId);
	rcb.Draw(3);
	rcb.EndRenderPass();
}