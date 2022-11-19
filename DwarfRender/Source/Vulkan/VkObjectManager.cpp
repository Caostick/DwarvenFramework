#include "VkObjectManager.h"

#define DEBUG_VK_OBJECT_DESTROY 0

#include <iostream>

namespace {
	template<typename TObjHandle>
	void DebugDestroyOutput(
		[[maybe_unused]] const char* objTypeName,
		[[maybe_unused]] TObjHandle objHandle) {
#if DEBUG_VK_OBJECT_DESTROY
		std::cout << "vk: Destroying "<< objTypeName << "( " << objHandle << " )\n";
#endif
	}
}

bool vk::RemovedObject::UpdateLifetime() {
	m_LifetimeLeft--;
	return m_LifetimeLeft <= 0;
}

vk::RemovedObject_Image::RemovedObject_Image(VkDevice device, VkImage image)
	: RemovedObject(device)
	, m_Image(image) {}

vk::RemovedObject_Image::~RemovedObject_Image() {
	DebugDestroyOutput("Image", m_Image);
	vk::DestroyImage(m_Device, m_Image);
}



vk::RemovedObject_ImageView::RemovedObject_ImageView(VkDevice device, VkImageView imageView) 
	: RemovedObject(device)
	, m_ImageView(imageView) {}

vk::RemovedObject_ImageView::~RemovedObject_ImageView() {
	DebugDestroyOutput("ImageView", m_ImageView);
	vk::DestroyImageView(m_Device, m_ImageView);
}



vk::RemovedObject_Buffer::RemovedObject_Buffer(VkDevice device, VkBuffer buffer)
	: RemovedObject(device)
	, m_Buffer(buffer) {}

vk::RemovedObject_Buffer::~RemovedObject_Buffer() {
	DebugDestroyOutput("Buffer", m_Buffer);
	vk::DestroyBuffer(m_Device, m_Buffer);
}



vk::RemovedObject_BufferView::RemovedObject_BufferView(VkDevice device, VkBufferView bufferView)
	: RemovedObject(device)
	, m_BufferView(bufferView) {}

vk::RemovedObject_BufferView::~RemovedObject_BufferView() {
	DebugDestroyOutput("BufferView", m_BufferView);
	vk::DestroyBufferView(m_Device, m_BufferView);
}



vk::RemovedObject_DeviceMemory::RemovedObject_DeviceMemory(VkDevice device, VkDeviceMemory deviceMemory)
	: RemovedObject(device)
	, m_DeviceMemory(deviceMemory) {}

vk::RemovedObject_DeviceMemory::~RemovedObject_DeviceMemory() {
	DebugDestroyOutput("DeviceMemory", m_DeviceMemory);
	vk::FreeMemory(m_Device, m_DeviceMemory);
}



vk::RemovedObject_Sampler::RemovedObject_Sampler(VkDevice device, VkSampler sampler)
	: RemovedObject(device)
	, m_Sampler(sampler) {}

vk::RemovedObject_Sampler::~RemovedObject_Sampler() {
	DebugDestroyOutput("Sampler", m_Sampler);
	vk::API::DestroySampler(m_Device, m_Sampler, vk::Allocator());
}



vk::RemovedObject_ShaderModule::RemovedObject_ShaderModule(VkDevice device, VkShaderModule shaderModule) 
	: RemovedObject(device)
	, m_ShaderModule(shaderModule) {}

vk::RemovedObject_ShaderModule::~RemovedObject_ShaderModule() {
	DebugDestroyOutput("ShaderModule", m_ShaderModule);
	vk::API::DestroyShaderModule(m_Device, m_ShaderModule, vk::Allocator());
}



vk::RemovedObject_RenderPass::RemovedObject_RenderPass(VkDevice device, VkRenderPass renderPass) 
	: RemovedObject(device)
	, m_RenderPass(renderPass) {}

vk::RemovedObject_RenderPass::~RemovedObject_RenderPass() {
	DebugDestroyOutput("RenderPass", m_RenderPass);
	vk::API::DestroyRenderPass(m_Device, m_RenderPass, vk::Allocator());
}



vk::RemovedObject_DescriptorSet::RemovedObject_DescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet)
	: RemovedObject(device)
	, m_DescriptorPool(descriptorPool)
	, m_DescriptorSet(descriptorSet) {}

vk::RemovedObject_DescriptorSet::~RemovedObject_DescriptorSet() {
	DebugDestroyOutput("DescriptorSet", m_DescriptorSet);
	vk::API::FreeDescriptorSets(m_Device, m_DescriptorPool, 1, &m_DescriptorSet);
}


vk::RemovedObject_DescriptorSetLayout::RemovedObject_DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout)
	: RemovedObject(device)
	, m_DescriptorSetLayout(descriptorSetLayout) {}

vk::RemovedObject_DescriptorSetLayout::~RemovedObject_DescriptorSetLayout() {
	DebugDestroyOutput("DescriptorSetLayout", m_DescriptorSetLayout);
	vk::API::DestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, vk::Allocator());
}



vk::RemovedObject_PipelineLayout::RemovedObject_PipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout)
	: RemovedObject(device)
	, m_PipelineLayout(pipelineLayout) {}

vk::RemovedObject_PipelineLayout::~RemovedObject_PipelineLayout() {
	DebugDestroyOutput("PipelineLayout", m_PipelineLayout);
	vk::API::DestroyPipelineLayout(m_Device, m_PipelineLayout, vk::Allocator());
}



vk::RemovedObject_Framebuffer::RemovedObject_Framebuffer(VkDevice device, VkFramebuffer framebuffer)
	: RemovedObject(device)
	, m_Framebuffer(framebuffer) {}

vk::RemovedObject_Framebuffer::~RemovedObject_Framebuffer() {
	DebugDestroyOutput("Framebuffer", m_Framebuffer);
	vk::API::DestroyFramebuffer(m_Device, m_Framebuffer, vk::Allocator());
}



vk::RemovedObject_Pipeline::RemovedObject_Pipeline(VkDevice device, VkPipeline pipeline) 
	: RemovedObject(device)
	, m_Pipeline(pipeline) {}

vk::RemovedObject_Pipeline::~RemovedObject_Pipeline() {
	DebugDestroyOutput("Pipeline", m_Pipeline);
	vk::API::DestroyPipeline(m_Device, m_Pipeline, vk::Allocator());
}



vk::RemovedObject_DescriptorPool::RemovedObject_DescriptorPool(VkDevice device, VkDescriptorPool descriptorPool) 
	: RemovedObject(device)
	, m_DescriptorPool(descriptorPool) {}

vk::RemovedObject_DescriptorPool::~RemovedObject_DescriptorPool() {
	DebugDestroyOutput("DescriptorPool", m_DescriptorPool);
	vk::API::DestroyDescriptorPool(m_Device, m_DescriptorPool, vk::Allocator());
}



vk::RemovedObject_CommandPool::RemovedObject_CommandPool(VkDevice device, VkCommandPool commandPool)
	: RemovedObject(device)
	, m_CommandPool(commandPool) {}

vk::RemovedObject_CommandPool::~RemovedObject_CommandPool() {
	DebugDestroyOutput("CommandPool", m_CommandPool);
	vk::API::DestroyCommandPool(m_Device, m_CommandPool, vk::Allocator());
}



vk::RemovedObject_Semaphore::RemovedObject_Semaphore(VkDevice device, VkSemaphore semaphore)
	: RemovedObject(device)
	, m_Semaphore(semaphore) {}

vk::RemovedObject_Semaphore::~RemovedObject_Semaphore() {
	DebugDestroyOutput("Semaphore", m_Semaphore);
	vk::API::DestroySemaphore(m_Device, m_Semaphore, vk::Allocator());
}



vk::RemovedObject_Fence::RemovedObject_Fence(VkDevice device, VkFence fence)
	: RemovedObject(device)
	, m_Fence(fence) {}

vk::RemovedObject_Fence::~RemovedObject_Fence() {
	DebugDestroyOutput("Fence", m_Fence);
	vk::API::DestroyFence(m_Device, m_Fence, vk::Allocator());
}



vk::RemovedObject_Swapchain::RemovedObject_Swapchain(VkDevice device, VkSwapchainKHR swapchain) 
	: RemovedObject(device)
	, m_Swapchain(swapchain) {}

vk::RemovedObject_Swapchain::~RemovedObject_Swapchain() {
	DebugDestroyOutput("Swapchain", m_Swapchain);
	vk::API::DestroySwapchainKHR(m_Device, m_Swapchain, vk::Allocator());
}



vk::RemovedObject_Surface::RemovedObject_Surface(VkInstance instance, VkSurfaceKHR surface)
	: RemovedObject(VK_NULL_HANDLE)
	, m_Instance(instance)
	, m_Surface(surface) {}

vk::RemovedObject_Surface::~RemovedObject_Surface() {
	DebugDestroyOutput("Surface", m_Surface);
	vk::API::DestroySurfaceKHR(m_Instance, m_Surface, vk::Allocator());
}




void vk::ObjectManager::RemoveImage(VkDevice device, VkImage image) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Image(device, image));
}

void vk::ObjectManager::RemoveImageView(VkDevice device, VkImageView imageView) {
	m_RemovedObjects.push_back(DFNew RemovedObject_ImageView(device, imageView));
}

void vk::ObjectManager::RemoveBuffer(VkDevice device, VkBuffer buffer) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Buffer(device, buffer));
}

void vk::ObjectManager::RemoveBufferView(VkDevice device, VkBufferView bufferView) {
	m_RemovedObjects.push_back(DFNew RemovedObject_BufferView(device, bufferView));
}

void vk::ObjectManager::RemoveDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory) {
	m_RemovedObjects.push_back(DFNew RemovedObject_DeviceMemory(device, deviceMemory));
}

void vk::ObjectManager::RemoveSampler(VkDevice device, VkSampler sampler) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Sampler(device, sampler));
}

void vk::ObjectManager::RemoveShaderModule(VkDevice device, VkShaderModule shaderModule) {
	m_RemovedObjects.push_back(DFNew RemovedObject_ShaderModule(device, shaderModule));
}

void vk::ObjectManager::RemoveRenderPass(VkDevice device, VkRenderPass renderPass) {
	m_RemovedObjects.push_back(DFNew RemovedObject_RenderPass(device, renderPass));
}

void vk::ObjectManager::RemoveDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet) {
	m_RemovedObjects.push_back(DFNew RemovedObject_DescriptorSet(device, descriptorPool, descriptorSet));
}

void vk::ObjectManager::RemoveDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout) {
	m_RemovedObjects.push_back(DFNew RemovedObject_DescriptorSetLayout(device, descriptorSetLayout));
}

void vk::ObjectManager::RemovePipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout) {
	m_RemovedObjects.push_back(DFNew RemovedObject_PipelineLayout(device, pipelineLayout));
}

void vk::ObjectManager::RemoveFramebuffer(VkDevice device, VkFramebuffer framebuffer) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Framebuffer(device, framebuffer));
}

void vk::ObjectManager::RemovePipeline(VkDevice device, VkPipeline pipeline) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Pipeline(device, pipeline));
}

void vk::ObjectManager::RemoveDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool) {
	m_RemovedObjects.push_back(DFNew RemovedObject_DescriptorPool(device, descriptorPool));
}

void vk::ObjectManager::RemoveCommandPool(VkDevice device, VkCommandPool commandPool) {
	m_RemovedObjects.push_back(DFNew RemovedObject_CommandPool(device, commandPool));
}

void vk::ObjectManager::RemoveSemaphore(VkDevice device, VkSemaphore semaphore) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Semaphore(device, semaphore));
}

void vk::ObjectManager::RemoveFence(VkDevice device, VkFence fence) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Fence(device, fence));
}

void vk::ObjectManager::RemoveSwapchain(VkDevice device, VkSwapchainKHR swapchain) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Swapchain(device, swapchain));
}

void vk::ObjectManager::RemoveSurface(VkInstance instance, VkSurfaceKHR surface) {
	m_RemovedObjects.push_back(DFNew RemovedObject_Surface(instance, surface));
}

bool vk::ObjectManager::Update() {
	for (auto obj : m_RemovedObjects) {
		if (obj->UpdateLifetime()) {
			DFDelete obj;
		} else {
			m_RemovedObjectsLeft.push_back(obj);
		}
	}

	m_RemovedObjects = m_RemovedObjectsLeft;
	m_RemovedObjectsLeft.clear();
	
	return !m_RemovedObjects.empty();
}

