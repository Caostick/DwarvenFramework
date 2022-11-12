#include "VkPresentation.h"
#include "VkAllocator.h"
#include "VkHelper.h"
#include "VkDebug.h"
#include "VkRenderCore.h"
#include "VkRenderPass.h"
#include "VkParameterSet.h"
#include "VkGraphicsPipeline.h"
#include "VkTexture.h"
#include "VkObjectManager.h"

#include "Generated/Presentation.generated.h"

#include <DwarfWindow/Window.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/DebugName.h>

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



vk::Presentation::Presentation(const df::Window& window)
	: m_VkSurface(VK_NULL_HANDLE)
	, m_VkSwapchain(VK_NULL_HANDLE)
	, m_VkExtent({ 0,0 })
	, m_Window(window)
	, m_VkFormat(VK_FORMAT_UNDEFINED)
	, m_VSyncEnabled(true)
	, m_ParametrSet(nullptr)
	, m_Pipeline(nullptr)
	, m_PresentTexture(nullptr)
	, m_MinImagesCount(0)
	, m_ImagesCount(0)
	, m_AvailableImageIndex(0) {}

auto vk::Presentation::GetWindow() const -> const df::Window& {
	return m_Window;
}

bool vk::Presentation::CreateSurface(VkInstance instance) {
	DFAssert(m_VkSurface == VK_NULL_HANDLE, "Surface already created!");

#ifdef GLFW_WINDOW_IMPLEMENTATION
	if (glfwCreateWindowSurface(instance, m_Window.GetPtr(), vk::Allocator(), &m_VkSurface) != VK_SUCCESS) {
		DFAssert(false, "Can't create Surface!");

		return false;
	}
#else
	static_assert(false, "No window API selected!");
#endif

	return true;
}

void vk::Presentation::DestroySurface(VkInstance instance) {
	DFAssert(m_VkSurface != VK_NULL_HANDLE, "Surface not created!");

	vk::API::DestroySurfaceKHR(instance, m_VkSurface, vk::Allocator());
	m_VkSurface = VK_NULL_HANDLE;
}

bool vk::Presentation::CreateSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	DFAssert(m_VkSurface != VK_NULL_HANDLE, "Surface wasn't created!");
	DFAssert(m_VkSwapchain == VK_NULL_HANDLE, "Swapchain already created!");

	const VkExtent2D extent = {
		m_Window.GetWidth(),
		m_Window.GetHeight()
	};

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

	m_ImageAvailableSemaphores.resize(m_ImagesCount);
	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		m_ImageAvailableSemaphores[i] = vk::CreateSemaphore(device, 0);
	}

	return true;
}

void vk::Presentation::DestroySwapchain(vk::RenderCore& renderCore) {
	DFAssert(m_VkSwapchain != VK_NULL_HANDLE, "Swapchain not created!");

	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		renderCore.RemoveSemaphore(m_ImageAvailableSemaphores[i]);
	}
	m_ImageAvailableSemaphores.clear();

	for (uint32 i = 0; i < m_ImagesCount; ++i) {
		renderCore.RemoveImageView(m_ImageViews[i]);
	}
	m_ImageViews.clear();

	m_Images.clear();

	renderCore.RemoveSwapchain(m_VkSwapchain);
	m_VkSwapchain = VK_NULL_HANDLE;
	m_VkExtent = VkExtent2D({ 0, 0 });
	m_MinImagesCount = 0;
	m_ImagesCount = 0;
}

auto vk::Presentation::GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex)->VkBool32 {
	VkBool32 presentSupport;
	vk::API::GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_VkSurface, &presentSupport);

	return presentSupport;
}

auto vk::Presentation::AquireNextImage(VkDevice device) -> VkResult {
	m_CurrentSemaphoreIndex = (m_CurrentSemaphoreIndex + 1) % m_ImagesCount;
	VkSemaphore vkSemaphore = m_ImageAvailableSemaphores[m_CurrentSemaphoreIndex];

	return vk::API::AcquireNextImageKHR(device, m_VkSwapchain, std::numeric_limits<uint64_t>::max(), vkSemaphore, VK_NULL_HANDLE, &m_AvailableImageIndex);
}

auto vk::Presentation::GetImageAvailableSemaphore()const->VkSemaphore {
	return m_ImageAvailableSemaphores[m_CurrentSemaphoreIndex];
}

auto vk::Presentation::Present(VkSemaphore semaphore, VkQueue presentQueue)->VkResult {
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

bool vk::Presentation::Load(vk::RenderCore& renderCore) {
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

		VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

		m_RenderPasses.resize(m_ImagesCount);
		for (uint32 i = 0; i < m_ImagesCount; ++i) {
			m_RenderPasses[i] = renderCore.CreateRenderPass();
			m_RenderPasses[i]->SetExtents(m_VkExtent.width, m_VkExtent.height);
			m_RenderPasses[i]->SetColorTarget(0, m_ImageViews[i], attachementDescription, clearValue);
			m_RenderPasses[i]->SetName(df::DebugName("Present [%d]", i));
		}
	}

	// Shaders
	{
		const char* vsCode =
			"layout(location = 0) out vec2 outTexcoord;\n"
			"\n"
			"vec2 positions[3] = vec2[](\n"
			"    vec2(-1.0, -1.0),\n"
			"    vec2(-1.0, 3.0),\n"
			"    vec2(3.0, -1.0)\n"
			");\n"
			"\n"
			"vec2 tcs[3] = vec2[](\n"
			"    vec2(0.0, 0.0),\n"
			"    vec2(0.0, 2.0),\n"
			"    vec2(2.0, 0.0)\n"
			");\n"
			"\n"
			"void main() {\n"
			"    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);\n"
			"    outTexcoord = tcs[gl_VertexIndex];\n"
			"}";

		const char* fsCode =
			"#ParameterSet Present\n"
			"\n"
			"layout(location = 0) in vec2 inTexcoord;\n"
			"\n"
			"layout(location = 0) out vec4 outColor;\n"
			"\n"
			"void main() {\n"
			"    outColor = texture(Texture, inTexcoord);\n"
			"}";

		m_ParametrSet = renderCore.CreateParameterSet("Present");
		m_ParametrSet->DeclareTextureParameter("Texture");
		m_ParametrSet->Build();

		m_Pipeline = renderCore.CreateGraphicsPipeline();
		m_Pipeline->SetName("Present");
		m_Pipeline->DeclareVertexShader(vsCode);
		m_Pipeline->DeclareFragmentShader(fsCode);
		m_Pipeline->Build();
	}

	return true;
}

void vk::Presentation::Unload(vk::RenderCore& renderCore) {
	renderCore.DestroyGraphicsPipeline(m_Pipeline);
	renderCore.DestroyParameterSet(m_ParametrSet);

	for (auto& renderPass : m_RenderPasses) {
		renderCore.DestroyRenderPass(renderPass);
	}
	m_RenderPasses.clear();
}

bool vk::Presentation::RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, bool vSyncEnabled, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	DestroySwapchain(renderCore);
	if (!CreateSwapchain(device, physicalDevice, vSyncEnabled, graphicsFamilyIndex, presentFamilyIndex)) {
		return false;
	}

	// Update render pass
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

		VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (uint32 i = 0; i < m_ImagesCount; ++i) {
			m_RenderPasses[i]->SetExtents(m_VkExtent.width, m_VkExtent.height);
			m_RenderPasses[i]->SetColorTarget(0, m_ImageViews[i], attachementDescription, clearValue);
		}
	}

	return true;
}

bool vk::Presentation::RecreateSwapchain(vk::RenderCore& renderCore, VkDevice device, VkPhysicalDevice physicalDevice, uint32 graphicsFamilyIndex, uint32 presentFamilyIndex) {
	return RecreateSwapchain(renderCore, device, physicalDevice, m_VSyncEnabled, graphicsFamilyIndex, presentFamilyIndex);
}

void vk::Presentation::SetPresentTexture(vk::Texture* texture) {
	m_PresentTexture = texture;
}

void vk::Presentation::PresentTexture(vk::CommandBuffer& rcb) {
	if (!m_PresentTexture) {
		return;
	}

	DFScopedRenderEvent(rcb, "Present Render Stage");

	m_ParametrSet->SetTexture("Texture", m_PresentTexture);
	m_ParametrSet->SetFilter("Texture", df::EFilter::Linear);

	rcb.BeginRenderPass(m_RenderPasses[m_AvailableImageIndex]);
	rcb.BindPipeline(m_Pipeline);
	rcb.BindParameterSet(m_ParametrSet);
	rcb.Draw(3);

	rcb.EndRenderPass();

#if 0
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
#endif
}