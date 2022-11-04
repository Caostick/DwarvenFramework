#pragma once

#include "VkAPI.h"
#include "VkHelper.h"
#include "VkAllocator.h"

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/New.h>

namespace vk {
	class RemovedObject {
	public:
		RemovedObject(VkDevice device) 
			: m_Device(device) {}
		virtual ~RemovedObject() {}

		bool UpdateLifetime();

	protected:
		VkDevice m_Device = VK_NULL_HANDLE;
		int m_LifetimeLeft = 3;
	};

	class RemovedObject_Image : public RemovedObject {
	public:
		RemovedObject_Image(VkDevice device, VkImage image);
		~RemovedObject_Image() override;
	private:
		VkImage m_Image;
	};

	class RemovedObject_ImageView : public RemovedObject {
	public:
		RemovedObject_ImageView(VkDevice device, VkImageView imageView);
		~RemovedObject_ImageView() override;
	private:
		VkImageView m_ImageView;
	};

	class RemovedObject_Buffer : public RemovedObject {
	public:
		RemovedObject_Buffer(VkDevice device, VkBuffer buffer);
		~RemovedObject_Buffer() override;
	private:
		VkBuffer m_Buffer;
	};

	class RemovedObject_BufferView : public RemovedObject {
	public:
		RemovedObject_BufferView(VkDevice device, VkBufferView bufferView);
		~RemovedObject_BufferView() override;
	private:
		VkBufferView m_BufferView;
	};

	class RemovedObject_DeviceMemory : public RemovedObject {
	public:
		RemovedObject_DeviceMemory(VkDevice device, VkDeviceMemory deviceMemory);
		~RemovedObject_DeviceMemory() override;
	private:
		VkDeviceMemory m_DeviceMemory;
	};

	class RemovedObject_Sampler : public RemovedObject {
	public:
		RemovedObject_Sampler(VkDevice device, VkSampler sampler);
		~RemovedObject_Sampler() override;
	private:
		VkSampler m_Sampler;
	};

	class RemovedObject_ShaderModule : public RemovedObject {
	public:
		RemovedObject_ShaderModule(VkDevice device, VkShaderModule shaderModule);
		~RemovedObject_ShaderModule() override;
	private:
		VkShaderModule m_ShaderModule;
	};

	class RemovedObject_RenderPass : public RemovedObject {
	public:
		RemovedObject_RenderPass(VkDevice device, VkRenderPass renderPass);
		~RemovedObject_RenderPass() override;
	private:
		VkRenderPass m_RenderPass;
	};

	class RemovedObject_DescriptorSet : public RemovedObject {
	public:
		RemovedObject_DescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet);
		~RemovedObject_DescriptorSet() override;
	private:
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet;
	};

	class RemovedObject_DescriptorSetLayout : public RemovedObject {
	public:
		RemovedObject_DescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);
		~RemovedObject_DescriptorSetLayout() override;
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};

	class RemovedObject_PipelineLayout : public RemovedObject {
	public:
		RemovedObject_PipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout);
		~RemovedObject_PipelineLayout() override;
	private:
		VkPipelineLayout m_PipelineLayout;
	};

	class RemovedObject_Framebuffer : public RemovedObject {
	public:
		RemovedObject_Framebuffer(VkDevice device, VkFramebuffer framebuffer);
		~RemovedObject_Framebuffer() override;
	private:
		VkFramebuffer m_Framebuffer;
	};

	class RemovedObject_Pipeline : public RemovedObject {
	public:
		RemovedObject_Pipeline(VkDevice device, VkPipeline pipeline);
		~RemovedObject_Pipeline() override;
	private:
		VkPipeline m_Pipeline;
	};

	class RemovedObject_DescriptorPool : public RemovedObject {
	public:
		RemovedObject_DescriptorPool(VkDevice device, VkDescriptorPool descriptorPool);
		~RemovedObject_DescriptorPool() override;
	private:
		VkDescriptorPool m_DescriptorPool;
	};

	class RemovedObject_CommandPool : public RemovedObject {
	public:
		RemovedObject_CommandPool(VkDevice device, VkCommandPool commandPool);
		~RemovedObject_CommandPool() override;
	private:
		VkCommandPool m_CommandPool;
	};

	class RemovedObject_Semaphore : public RemovedObject {
	public:
		RemovedObject_Semaphore(VkDevice device, VkSemaphore semaphore);
		~RemovedObject_Semaphore() override;
	private:
		VkSemaphore m_Semaphore;
	};

	class RemovedObject_Fence : public RemovedObject {
	public:
		RemovedObject_Fence(VkDevice device, VkFence fence);
		~RemovedObject_Fence() override;
	private:
		VkFence m_Fence;
	};






	class ObjectManager {
	public:
		void RemoveImage(VkDevice device, VkImage image);
		void RemoveImageView(VkDevice device, VkImageView imageView);
		void RemoveBuffer(VkDevice device, VkBuffer buffer);
		void RemoveBufferView(VkDevice device, VkBufferView bufferView);
		void RemoveDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory);
		void RemoveSampler(VkDevice device, VkSampler sampler);
		void RemoveShaderModule(VkDevice device, VkShaderModule shaderModule);
		void RemoveRenderPass(VkDevice device, VkRenderPass renderPass);
		void RemoveDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet);
		void RemoveDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);
		void RemovePipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout);
		void RemoveFramebuffer(VkDevice device, VkFramebuffer framebuffer);
		void RemovePipeline(VkDevice device, VkPipeline pipeline);
		void RemoveDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool);
		void RemoveCommandPool(VkDevice device, VkCommandPool commandPool);
		void RemoveSemaphore(VkDevice device, VkSemaphore semaphore);
		void RemoveFence(VkDevice device, VkFence fence);

		bool Update();

	private:
		df::Vector<RemovedObject*> m_RemovedObjects;
		df::Vector<RemovedObject*> m_RemovedObjectsLeft;
	};
}