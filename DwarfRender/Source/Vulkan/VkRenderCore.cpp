#include "VkRenderCore.h"
#include "VkInit.h"
#include "VkAllocator.h"
#include "VkDefinitions.h"
#include "VkDebug.h"
#include "VkHelper.h"
#include "VkRenderPass.h"
#include "VkGraphicsPipeline.h"
#include "VkParameterSetDefinition.h"
#include "VkParameterSet.h"
#include "VkShaderCompiler.h"
#include "VkBuffer.h"
#include "VkMesh.h"
#include "VkTexture.h"

#include <DwarfWindow/Window.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Memory.h>

vk::RenderCore::RenderCore() 
	: m_ShaderCompiler(nullptr) {

	m_NumFramesInFlight = 0;
	m_FrameIndex = 0;

	m_VkInstance = VK_NULL_HANDLE;
	m_VkPhysicalDevice = VK_NULL_HANDLE;
	m_VkDevice = VK_NULL_HANDLE;

	m_PresentFamilyIndex = 0;
	m_GraphicsFamilyIndex = 0;
	m_TransferFamilyIndex = 0;
	m_ComputeFamilyIndex = 0;

	m_PresentQueue = VK_NULL_HANDLE;
	m_GraphicsQueue = VK_NULL_HANDLE;
	m_TransferQueue = VK_NULL_HANDLE;
	m_ComputeQueue = VK_NULL_HANDLE;

	m_DescriptorPool = VK_NULL_HANDLE;

	m_DeviceProperties = {};
	m_DeviceFeatures = {};
	m_DeviceLimits = {};

	//m_VulkanObjectManager;
}

auto vk::RenderCore::GetVkDevice() const->VkDevice {
	return m_VkDevice;
}

auto vk::RenderCore::GetVkDescriptorPool() const->VkDescriptorPool {
	return m_DescriptorPool;
}

bool vk::RenderCore::Init() {
	AddShaderInclude("", "");

	m_ShaderCompiler = DFNew vk::ShaderCompiler(*this);

	m_NumFramesInFlight = 3;

	m_FrameData.resize(m_NumFramesInFlight);
	m_FrameIndex = 0;

	if (!vk::API::Load()) {
		return false;
	}

	if (!InitInstance()) {
		return false;
	}

	if (!InitPhysicalDevice()) {
		return false;
	}

	if (!CheckForPreventedCaptureSoft()) {
		return false;
	}

	if (!InitDevice()) {
		return false;
	}

	vk::API::GetDeviceQueue(m_VkDevice, m_PresentFamilyIndex, 0, &m_PresentQueue);
	vk::API::GetDeviceQueue(m_VkDevice, m_GraphicsFamilyIndex, 0, &m_GraphicsQueue);
	vk::API::GetDeviceQueue(m_VkDevice, m_TransferFamilyIndex, 0, &m_TransferQueue);
	vk::API::GetDeviceQueue(m_VkDevice, m_ComputeFamilyIndex, 0, &m_ComputeQueue);

	if (!InitCommandPools()) {
		return false;
	}

	if (!InitDescriptorPools()) {
		return false;
	}

	for (auto& frameData : m_FrameData) {
		if (!InitFrameData(frameData)) {
			return false;
		}
	}

	// Transfer
	m_TransferCommandBuffer.Init(df::DebugName("TransferCommandBuffer"), m_VkDevice, m_TransferCommandPools[0]);

	m_TransferBuffer.Init(m_VkDevice, 1024 * 1024 * 32);

	return true;
}

void vk::RenderCore::Release() {
	m_TransferBuffer.Release(m_VulkanObjectManager);
	EmptyTrashCan();

	m_TransferCommandBuffer.Release(m_VkDevice, m_TransferCommandPools[0]);

	for (auto& frameData : m_FrameData) {
		ReleaseFrameData(frameData);
	}
	m_FrameData.clear();

	ReleaseDescriptorPools();

	ReleaseCommandPools();

	m_VertexAttributes.Clear();

	for (auto sampler : m_Samplers) {
		m_VulkanObjectManager.RemoveSampler(m_VkDevice, sampler->GetVkSampler());
	}
	m_Samplers.Clear();

	EmptyTrashCan();

	// Destroy device and physical device
	DFAssert(m_VkDevice != VK_NULL_HANDLE, "Device not created!");
	vk::API::DestroyDevice(m_VkDevice, vk::Allocator());
	m_VkDevice = VK_NULL_HANDLE;
	m_VkPhysicalDevice = VK_NULL_HANDLE;

	// Destroy instance
	DFAssert(m_VkInstance != VK_NULL_HANDLE, "Instance not created!");
	vk::DestroyDebugCallback(m_VkInstance);
	vk::API::DestroyInstance(m_VkInstance, vk::Allocator());
	m_VkInstance = VK_NULL_HANDLE;

	DFDelete m_ShaderCompiler;
	m_ShaderCompiler = nullptr;
}

void vk::RenderCore::SetWindowSource(df::Window* window, vk::Texture* texture) {
	for (auto presentation : m_Presentations) {
		if (&presentation->GetWindow() == window) {
			presentation->SetPresentTexture(texture);
			return;
		}
	}

	auto presentation = m_Presentations.Create(*window);
	presentation->SetPresentTexture(texture);

	if (!presentation->CreateSurface(m_VkInstance)) {
		DFAssert(false, "Can't create window surface!");
	}
	// Destroy surface
	//presentation->DestroySurface(m_VkInstance);
	
	if (!presentation->CreateSwapchain(
		m_VkDevice, m_VkPhysicalDevice, true,
		m_GraphicsFamilyIndex, m_PresentFamilyIndex)) {
		DFAssert(false, "Can't create swapchain!");
	}

	// Destroy swapchain
	//presentation->DestroySwapchain(m_VkDevice);

	presentation->Load(*this);
	//presentation->Unload(*this);

}

auto vk::RenderCore::BeginFrame() ->vk::CommandBuffer* {
	m_VulkanObjectManager.Update();

	vk::FrameData& frame = m_FrameData[m_FrameIndex];

	vk::API::WaitForFences(m_VkDevice, 1, &frame.m_InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());

	m_ValidPresentations.clear();
	for (auto presentation : m_Presentations) {
		if (presentation->GetWindow().GetWidth() == 0 || 
			presentation->GetWindow().GetHeight() == 0) {
			continue;
		}

		const VkResult result = presentation->AquireNextImage(m_VkDevice);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			presentation->RecreateSwapchain(
				*this, m_VkDevice, m_VkPhysicalDevice,
				m_GraphicsFamilyIndex, m_PresentFamilyIndex
			);
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			DFAssert(false, "Failed to aquire swap chain image!");
		} else {
			m_ValidPresentations.push_back(presentation);
		}
	}

	vk::API::ResetFences(m_VkDevice, 1, &frame.m_InFlightFence);

	frame.m_CommandBuffer.Begin();

	// Init texture layouts
	for (auto texture : m_TexturesToInitLayout) {
		if (texture->IsDepthStencil()) {
			frame.m_CommandBuffer.ImageLayoutTransition(texture, vk::EImageLayout::Undefined, vk::EImageLayout::DepthStencilReadOnly);
		} else {
			frame.m_CommandBuffer.ImageLayoutTransition(texture, vk::EImageLayout::Undefined, vk::EImageLayout::ColorReadOnly);
		}
	}
	m_TexturesToInitLayout.clear();

	// Transfer
	{
		DFScopedRenderEvent(frame.m_CommandBuffer, "Transfer");

		m_TransferBuffer.Execute(frame.m_CommandBuffer);
	}

	// Generate Mips
	for (auto texture : m_TexturesToGenerateMips) {
		frame.m_CommandBuffer.GenerateMips(texture);
	}
	m_TexturesToGenerateMips.clear();

	return &frame.m_CommandBuffer;
}

void vk::RenderCore::EndFrame() {

	vk::FrameData& frame = m_FrameData[m_FrameIndex];

	for (auto presentation : m_ValidPresentations) {
		presentation->PresentTexture(frame.m_CommandBuffer);
	}

	frame.m_CommandBuffer.End();

	const VkSemaphore signalSemaphores[] = { frame.m_RenderFinishedSemaphore };
	const VkCommandBuffer commandBuffer = frame.m_CommandBuffer.Get();

	const bool hasPresentations = !m_ValidPresentations.empty();

	df::Vector<VkSemaphore> imageAvailableSemaphores(m_ValidPresentations.size());
	df::Vector<VkPipelineStageFlags> waitStages(m_ValidPresentations.size());

	for (size_t i = 0; i < m_ValidPresentations.size(); ++i) {
		imageAvailableSemaphores[i] = m_ValidPresentations[i]->GetImageAvailableSemaphore();
		waitStages[i] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = uint32(m_ValidPresentations.size());
	submitInfo.pWaitSemaphores = hasPresentations ? imageAvailableSemaphores.data() : nullptr;
	submitInfo.pWaitDstStageMask = hasPresentations ? waitStages.data() : nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = hasPresentations ? 1 : 0;
	submitInfo.pSignalSemaphores = hasPresentations ? signalSemaphores : nullptr;

	if (vk::API::QueueSubmit(m_GraphicsQueue, 1, &submitInfo, frame.m_InFlightFence) != VK_SUCCESS) {
		DFAssert(false, "Failed to submit render command buffer!");
	}

	for (auto presentation : m_ValidPresentations) {
		const VkResult result = presentation->Present(frame.m_RenderFinishedSemaphore, m_PresentQueue);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			presentation->RecreateSwapchain(
				*this, m_VkDevice, m_VkPhysicalDevice,
				m_GraphicsFamilyIndex, m_PresentFamilyIndex
			);
		} else if (result != VK_SUCCESS) {
			DFAssert(false, "Failed to present swap chain image!");
		}
	}
	m_ValidPresentations.clear();

	m_FrameIndex = (m_FrameIndex + 1) % m_NumFramesInFlight;
}

void vk::RenderCore::CompleteAllCommands() {
	vk::API::QueueWaitIdle(m_GraphicsQueue);
	for (int i = 0; i < m_FrameData.size(); ++i) {
		vk::API::WaitForFences(m_VkDevice, 1, &m_FrameData[i].m_InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	}

	EmptyTrashCan();
}

auto vk::RenderCore::CreateRenderPass()->vk::RenderPass* {
	return m_RenderPasses.Create(*this);
}

void vk::RenderCore::DestroyRenderPass(vk::RenderPass* renderPass) {
	m_RenderPasses.Destroy(renderPass);
}

auto vk::RenderCore::CreateGraphicsPipeline()->vk::GraphicsPipeline* {
	return m_GraphicsPipelines.Create(*this);
}

void vk::RenderCore::DestroyGraphicsPipeline(vk::GraphicsPipeline* pipeline) {
	m_GraphicsPipelines.Destroy(pipeline);
}

auto vk::RenderCore::CreateBuffer()->vk::Buffer* {
	return m_Buffers.Create(*this);
}

void vk::RenderCore::DestroyBuffer(vk::Buffer* buffer) {
	m_Buffers.Destroy(buffer);
}

auto vk::RenderCore::CreateMesh()->vk::Mesh* {
	return m_Meshes.Create(*this);
}

void vk::RenderCore::DestroyMesh(vk::Mesh* mesh) {
	m_Meshes.Destroy(mesh);
}

auto vk::RenderCore::CreateTexture()->vk::Texture* {
	auto texture = m_Textures.Create(*this);

	m_TexturesToInitLayout.push_back(texture);

	return texture;
}

void vk::RenderCore::DestroyTexture(vk::Texture* texture) {
	m_Textures.Destroy(texture);
}

auto vk::RenderCore::CreateParameterSetDefinition(const df::StringView& name)->vk::ParameterSetDefinition* {
	const df::String strName = df::String(name);
	auto it = m_ParameterSetDefinitionRegistry.find(strName);
	if (it != m_ParameterSetDefinitionRegistry.end()) {
		DFAssert(false, "Definition with given name already exists!");
		return it->second;
	}

	auto def = m_ParameterSetDefinitions.Create(*this, name);

	m_ParameterSetDefinitionRegistry[strName] = def;

	return def;
}

void vk::RenderCore::DestroyParameterSetDefinition(vk::ParameterSetDefinition* parameterSetDefinition) {
	const df::String& strName = parameterSetDefinition->GetName();
	auto it = m_ParameterSetDefinitionRegistry.find(strName);
	if (it != m_ParameterSetDefinitionRegistry.end() && it->second == parameterSetDefinition) {
		m_ParameterSetDefinitionRegistry.erase(it);
	}

	m_ParameterSetDefinitions.Destroy(parameterSetDefinition);
}

auto vk::RenderCore::FindParameterSetDefinition(const df::StringView& name) const -> const vk::ParameterSetDefinition* {
	const df::String strName = df::String(name);
	auto it = m_ParameterSetDefinitionRegistry.find(strName);
	if (it == m_ParameterSetDefinitionRegistry.end()) {
		return nullptr;
	}

	return it->second;
}

auto vk::RenderCore::CreateParameterSet(const df::StringView& className)->vk::ParameterSet* {
	auto parameterSetDefinition = CreateParameterSetDefinition(className);
	return CreateParameterSet(parameterSetDefinition);
}

auto vk::RenderCore::CreateParameterSet(vk::ParameterSetDefinition* parameterSetDefinition)->vk::ParameterSet* {
	return m_ParameterSets.Create(*this, *parameterSetDefinition);
}

void vk::RenderCore::DestroyParameterSet(vk::ParameterSet* parameterSet) {
	m_ParameterSets.Destroy(parameterSet);
}

auto vk::RenderCore::RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format) -> const vk::VertexAttribute* {
	for (auto attr : m_VertexAttributes) {
		if (attr->m_Name == name) {
			DFAssert(attr->m_Format == format, "Vertext attribure is already registered with different format!");
			return attr;
		}
	}

	const uint32 index = m_VertexAttributes.Count();

	auto attr = m_VertexAttributes.Create();
	attr->m_Name = df::String(name);
	attr->m_Format = format;
	attr->m_Index = index;

	return attr;
}

auto vk::RenderCore::FindVertexAttribute(const df::StringView& name) const -> const vk::VertexAttribute* {
	for (auto attr : m_VertexAttributes) {
		if (attr->m_Name == name) {
			return attr;
		}
	}
	return nullptr;
}

void vk::RenderCore::AddShaderInclude(const df::StringView& name, const df::StringView& content) {
	m_ShaderIncludes[df::String(name)] = df::String(content);
}

auto vk::RenderCore::GetShaderInclude(const df::StringView& name) const->const df::String& {
	const auto& it = m_ShaderIncludes.find(df::String(name));
	if (it != m_ShaderIncludes.end()) {
		return it->second;
	}

	return m_NullInclude;
}

auto vk::RenderCore::GetShaderCompiler()->vk::ShaderCompiler* {
	return m_ShaderCompiler;
}

auto vk::RenderCore::RequestSampler(const vk::SamplerState& state)->vk::Sampler* {
	for (auto sampler : m_Samplers) {
		if (*sampler == state) {
			return sampler;
		}
	}

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = state.m_VkFilter;
	samplerCreateInfo.minFilter = state.m_VkFilter;
	samplerCreateInfo.addressModeU = state.m_VkAddressMode;
	samplerCreateInfo.addressModeV = state.m_VkAddressMode;
	samplerCreateInfo.addressModeW = state.m_VkAddressMode;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;
	samplerCreateInfo.maxAnisotropy = 16;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.mipmapMode = state.m_VkMipMapMode;
	samplerCreateInfo.minLod = 0;
	samplerCreateInfo.maxLod = 12;

	VkSampler vkSampler = VK_NULL_HANDLE;
	if (vk::API::CreateSampler(m_VkDevice, &samplerCreateInfo, vk::Allocator(), &vkSampler) != VK_SUCCESS) {
		DFAssert(false, "Can't create Sampler!");
	}

	DFVkDebugName(m_VkDevice, vkSampler, "Sampler");

	return m_Samplers.Create(state, vkSampler);
}

void vk::RenderCore::RemoveImage(VkImage image) {
	m_VulkanObjectManager.RemoveImage(m_VkDevice, image);
}

void vk::RenderCore::RemoveImageView(VkImageView imageView) {
	m_VulkanObjectManager.RemoveImageView(m_VkDevice, imageView);
}

void vk::RenderCore::RemoveBuffer(VkBuffer buffer) {
	m_VulkanObjectManager.RemoveBuffer(m_VkDevice, buffer);
}

void vk::RenderCore::RemoveBufferView(VkBufferView bufferView) {
	m_VulkanObjectManager.RemoveBufferView(m_VkDevice, bufferView);
}

void vk::RenderCore::RemoveDeviceMemory(VkDeviceMemory deviceMemory) {
	m_VulkanObjectManager.RemoveDeviceMemory(m_VkDevice, deviceMemory);
}

void vk::RenderCore::RemoveSampler(VkSampler sampler) {
	m_VulkanObjectManager.RemoveSampler(m_VkDevice, sampler);
}

void vk::RenderCore::RemoveShaderModule(VkShaderModule shaderModule) {
	m_VulkanObjectManager.RemoveShaderModule(m_VkDevice, shaderModule);
}

void vk::RenderCore::RemoveRenderPass(VkRenderPass renderPass) {
	m_VulkanObjectManager.RemoveRenderPass(m_VkDevice, renderPass);
}

void vk::RenderCore::RemoveDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet) {
	m_VulkanObjectManager.RemoveDescriptorSet(m_VkDevice, descriptorPool, descriptorSet);
}

void vk::RenderCore::RemoveDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout) {
	m_VulkanObjectManager.RemoveDescriptorSetLayout(m_VkDevice, descriptorSetLayout);
}

void vk::RenderCore::RemovePipelineLayout(VkPipelineLayout pipelineLayout) {
	m_VulkanObjectManager.RemovePipelineLayout(m_VkDevice, pipelineLayout);
}

void vk::RenderCore::RemoveFramebuffer(VkFramebuffer framebuffer) {
	m_VulkanObjectManager.RemoveFramebuffer(m_VkDevice, framebuffer);
}

void vk::RenderCore::RemovePipeline(VkPipeline pipeline) {
	m_VulkanObjectManager.RemovePipeline(m_VkDevice, pipeline);
}

void vk::RenderCore::RemoveDescriptorPool(VkDescriptorPool descriptorPool) {
	m_VulkanObjectManager.RemoveDescriptorPool(m_VkDevice, descriptorPool);
}

void vk::RenderCore::RemoveCommandPool(VkCommandPool commandPool) {
	m_VulkanObjectManager.RemoveCommandPool(m_VkDevice, commandPool);
}

void vk::RenderCore::RemoveSwapchain(VkSwapchainKHR swapchain) {
	m_VulkanObjectManager.RemoveSwapchain(m_VkDevice, swapchain);
}

void vk::RenderCore::RemoveSemaphore(VkSemaphore semaphore) {
	m_VulkanObjectManager.RemoveSemaphore(m_VkDevice, semaphore);
}

void vk::RenderCore::SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset /*= 0*/) {
	m_TransferBuffer.SetBufferData(buffer, data, dataSize, offset);
}

void vk::RenderCore::SetImageData(VkImage image, const void* data, uint32 dataSize, uint32 width, uint32 height, int32 widthOffset /*= 0*/, int32 heightOffset /*= 0*/) {
	m_TransferBuffer.SetImageData(image, data, dataSize, width, height, widthOffset, heightOffset);
}

void vk::RenderCore::GenerateMips(vk::Texture* texture) {
	m_TexturesToGenerateMips.push_back(texture);
}

auto vk::RenderCore::GetBufferAlignment(const BufferUsageFlags& usage)const->uint32 {
	uint32 alignment = 0;

	if (usage.Has(EBufferUsageFlag::Storage)) {
		alignment = std::max(alignment, uint32(m_DeviceLimits.minStorageBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::Uniform)) {
		alignment = std::max(alignment, uint32(m_DeviceLimits.minUniformBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::Vertex)) {
		alignment = std::max(alignment, uint32(sizeof(float)));
	}

	if (usage.Has(EBufferUsageFlag::Index)) {
		alignment = std::max(alignment, uint32(sizeof(uint32)));
	}

	if (usage.Has(EBufferUsageFlag::Indirect)) {
		alignment = std::max(alignment, uint32(sizeof(uint32)));
	}

	if (usage.Has(EBufferUsageFlag::StorageTexel)) {
		alignment = std::max(alignment, uint32(m_DeviceLimits.minStorageBufferOffsetAlignment));
		alignment = std::max(alignment, uint32(m_DeviceLimits.minTexelBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::UniformTexel)) {
		alignment = std::max(alignment, uint32(m_DeviceLimits.minUniformBufferOffsetAlignment));
		alignment = std::max(alignment, uint32(m_DeviceLimits.minTexelBufferOffsetAlignment));
	}

	return alignment;
}

bool vk::RenderCore::InitInstance() {
	DFAssert(m_VkInstance == VK_NULL_HANDLE, "Instance already created!");

	VkApplicationInfo applicationCreateInfo = {};
	applicationCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationCreateInfo.pApplicationName = "Game";
	applicationCreateInfo.pEngineName = "Engine";
	applicationCreateInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationCreateInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationCreateInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationCreateInfo;
	instanceCreateInfo.ppEnabledExtensionNames = vk::GetRequiredInstanceExtensions();
	instanceCreateInfo.enabledExtensionCount = vk::GetNumRequiredInstanceExtensions();
	instanceCreateInfo.ppEnabledLayerNames = vk::GetRequiredValidationLayers();
	instanceCreateInfo.enabledLayerCount = vk::GetNumRequiredValidationLayers();

	const VkResult result = vk::API::CreateInstance(&instanceCreateInfo, vk::Allocator(), &m_VkInstance);
	if (result != VK_SUCCESS) {
		DFAssert(false, vk::GetErrorString(result));

		return false;
	}

	vk::API::LoadInstanceFunctions(m_VkInstance);

	if (!vk::CreateDebugCallback(m_VkInstance)) {
		return false;
	}

	return true;
}

bool vk::RenderCore::InitPhysicalDevice() {
	uint32 physicalDeviceCount = 0;
	vk::API::EnumeratePhysicalDevices(m_VkInstance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		DFAssert(false, "Failed to find any physical device!");

		return false;
	}

	df::Vector<VkPhysicalDevice> devices = df::Vector<VkPhysicalDevice>(physicalDeviceCount);
	vk::API::EnumeratePhysicalDevices(m_VkInstance, &physicalDeviceCount, devices.data());

	for (uint32 i = 0; i < physicalDeviceCount; ++i) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vk::API::GetPhysicalDeviceProperties(devices[i], &deviceProperties);
		vk::API::GetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			m_VkPhysicalDevice = devices[i];

			m_DeviceProperties = deviceProperties;
			m_DeviceFeatures = deviceFeatures;
			m_DeviceLimits = deviceProperties.limits;
		}
	}

	if (m_VkPhysicalDevice == VK_NULL_HANDLE) {
		DFAssert(false, "There is no compatible PhysicalDevice!");

		return false;
	}


	uint32 queueFamilyCount = 0;
	vk::API::GetPhysicalDeviceQueueFamilyProperties(m_VkPhysicalDevice, &queueFamilyCount, nullptr);

	if (queueFamilyCount == 0) {
		DFAssert(false, "Failed to find any QueueFamily!");

		return false;
	}

	df::Vector<VkQueueFamilyProperties> queueFamilies = df::Vector<VkQueueFamilyProperties>(queueFamilyCount);
	vk::API::GetPhysicalDeviceQueueFamilyProperties(m_VkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	for (uint32 i = 0; i < queueFamilyCount; ++i) {
		VkQueueFamilyProperties queueFamily = queueFamilies[i];

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			m_GraphicsFamilyIndex = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			m_TransferFamilyIndex = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			m_ComputeFamilyIndex = i;
		}

		const VkBool32 presentSupport = true;// m_Presentation.GetPhysicalDeviceSurfaceSupport(m_VkPhysicalDevice, i);

		if (queueFamily.queueCount > 0 && presentSupport) {
			m_PresentFamilyIndex = i;
		}

		if (m_PresentFamilyIndex != uint32(-1) &&
			m_GraphicsFamilyIndex != uint32(-1) &&
			m_TransferFamilyIndex != uint32(-1) &&
			m_ComputeFamilyIndex != uint32(-1)) {
			vk::SetupMemoryProperties(m_VkPhysicalDevice);
			break;
		}
	}

	return true;
}

bool vk::RenderCore::InitDevice() {
	DFAssert(m_VkDevice == VK_NULL_HANDLE, "Device already created!");

	uint32 uniqueQueues[vk::Constants::MaxQueueFamilyIndices];

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		uniqueQueues[i] = uint32(-1);
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_PresentFamilyIndex) {
			uniqueQueues[i] = m_PresentFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_GraphicsFamilyIndex) {
			uniqueQueues[i] = m_GraphicsFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_TransferFamilyIndex) {
			uniqueQueues[i] = m_TransferFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_ComputeFamilyIndex) {
			uniqueQueues[i] = m_ComputeFamilyIndex;
			break;
		}
	}

	float queuePriority = 1.0f;
	uint32 queueFamilyCount = 0;
	VkDeviceQueueCreateInfo queueCreateInfos[vk::Constants::MaxQueueFamilyIndices];
	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i, ++queueFamilyCount) {
		if (uniqueQueues[i] == uint32(-1)) {
			break;
		}

		queueCreateInfos[i] = {};
		queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfos[i].queueFamilyIndex = uniqueQueues[i];
		queueCreateInfos[i].queueCount = 1;
		queueCreateInfos[i].pQueuePriorities = &queuePriority;
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	vk::API::GetPhysicalDeviceFeatures(m_VkPhysicalDevice, &deviceFeatures);

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = queueFamilyCount;
	createInfo.pQueueCreateInfos = queueCreateInfos;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = vk::GetNumRequiredDeviceExtensions();
	createInfo.ppEnabledExtensionNames = vk::GetRequiredDeviceExtensions();
	createInfo.enabledLayerCount = vk::GetNumRequiredValidationLayers();
	createInfo.ppEnabledLayerNames = vk::GetRequiredValidationLayers();

	if (vk::API::CreateDevice(m_VkPhysicalDevice, &createInfo, vk::Allocator(), &m_VkDevice) != VK_SUCCESS) {
		DFAssert(false, "Can't create Device!");

		return false;
	}

	if (!vk::API::LoadDeviceFunctions(m_VkDevice)) {
		return false;
	}

	return true;
}

bool vk::RenderCore::CheckForPreventedCaptureSoft() {
#define DF_PREVENT_RENDERDOC 0
#if DF_PREVENT_RENDERDOC
	df::Vector<df::String> preventedLayers = {
		"VK_LAYER_RENDERDOC_Capture"
	};

	uint32 layerPropertyCount = 0;
	df::Vector<VkLayerProperties> layerProperties;
	vk::API::EnumerateDeviceLayerProperties(m_APIData.m_VkPhysicalDevice, &layerPropertyCount, nullptr);
	layerProperties.resize(layerPropertyCount);
	vk::API::EnumerateDeviceLayerProperties(m_APIData.m_VkPhysicalDevice, &layerPropertyCount, layerProperties.data());

	for (const auto& layer : layerProperties) {
		for (const auto& preventedLayer : preventedLayers) {
			if (preventedLayer == layer.layerName) {
				return false;
			}
		}
	}
#endif

	return true;
}


bool vk::RenderCore::InitCommandPools() {
	DFAssert(m_GraphicsCommandPools.empty(), "GraphicsCommandPools already created!");
	DFAssert(m_TransferCommandPools.empty(), "TransferCommandPools already created!");

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	// Graphics
	{
		poolInfo.queueFamilyIndex = m_GraphicsFamilyIndex;
		m_GraphicsCommandPools.resize(1);
		for (VkCommandPool& commandPool : m_GraphicsCommandPools) {
			if (vk::API::CreateCommandPool(m_VkDevice, &poolInfo, vk::Allocator(), &commandPool) != VK_SUCCESS) {
				DFAssert(false, "Can't create GraphicsCommandPool!");

				return false;
			}

			DFVkDebugName(m_VkDevice, commandPool, "GraphicsCommandPool");
		}
	}

	// Transfer
	{
		poolInfo.queueFamilyIndex = m_TransferFamilyIndex;
		m_TransferCommandPools.resize(1);
		for (VkCommandPool& commandPool : m_TransferCommandPools) {
			if (vk::API::CreateCommandPool(m_VkDevice, &poolInfo, vk::Allocator(), &commandPool) != VK_SUCCESS) {
				DFAssert(false, "Can't create TransferCommandPool!");

				return false;
			}

			DFVkDebugName(m_VkDevice, commandPool, "TransferCommandPool");
		}
	}

	return true;
}

void vk::RenderCore::ReleaseCommandPools() {
	for (VkCommandPool commandPool : m_GraphicsCommandPools) {
		m_VulkanObjectManager.RemoveCommandPool(m_VkDevice, commandPool);
	}
	m_GraphicsCommandPools.clear();

	for (VkCommandPool commandPool : m_TransferCommandPools) {
		m_VulkanObjectManager.RemoveCommandPool(m_VkDevice, commandPool);
	}
	m_TransferCommandPools.clear();
}

bool vk::RenderCore::InitDescriptorPools() {

	const uint32 maxDescriptorSets = 800;
	df::Vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , 3 });

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
	descriptorPoolCreateInfo.maxSets = maxDescriptorSets;


	if (vk::API::CreateDescriptorPool(m_VkDevice, &descriptorPoolCreateInfo, vk::Allocator(), &m_DescriptorPool) != VK_SUCCESS) {
		DFAssert(false, "Can't create DescriptorPool!");
		return false;
	}

	return true;
}

void vk::RenderCore::ReleaseDescriptorPools() {
	m_VulkanObjectManager.RemoveDescriptorPool(m_VkDevice, m_DescriptorPool);
}


bool vk::RenderCore::InitFrameData(vk::FrameData& frameData) {
	if (!InitCommandBuffers(frameData)) {
		return false;
	}

	frameData.m_RenderFinishedSemaphore = vk::CreateSemaphore(m_VkDevice, 0);

	frameData.m_InFlightFence = vk::CreateFence(m_VkDevice, VK_FENCE_CREATE_SIGNALED_BIT);

	return true;
}

void vk::RenderCore::ReleaseFrameData(vk::FrameData& frameData) {
	m_VulkanObjectManager.RemoveFence(m_VkDevice, frameData.m_InFlightFence);

	m_VulkanObjectManager.RemoveSemaphore(m_VkDevice, frameData.m_RenderFinishedSemaphore);

	ReleaseCommandBuffers(frameData);
}

bool vk::RenderCore::InitCommandBuffers(vk::FrameData& frameData) {
	DFAssert(!m_GraphicsCommandPools.empty(), "GraphicsCommandPools not created!");
	DFAssert(!m_TransferCommandPools.empty(), "TransferCommandPools not created!");

	// Graphics
	frameData.m_CommandBuffer.Init("GraphicsCommandBuffer", m_VkDevice, m_GraphicsCommandPools[0]);

	return true;
}

void vk::RenderCore::ReleaseCommandBuffers(vk::FrameData& frameData) {
	frameData.m_CommandBuffer.Release(m_VkDevice, m_GraphicsCommandPools[0]);
}

void vk::RenderCore::EmptyTrashCan() {
	while (m_VulkanObjectManager.Update()) {}
}