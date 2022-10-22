#pragma once

#include "VkAPI.h"
#include "VkObjectManager.h"
#include "VkTransferBuffer.h"
#include "VkFrameData.h"
#include "VkPresentation.h"
#include "VkVertexAttribute.h"

#include <DwarvenCore/Types.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/HashMap.h>

namespace df {
	class Window;
}

namespace vk {
	struct RenderContext;

	class RenderPass;
	class Pipeline;
	class ParameterSetDefinition;
	class ParameterSet;
	class ShaderCompiler;
}

namespace vk {
	class RenderCore {
	public:
		RenderCore(const df::Window& window);

		auto GetVkDevice() const->VkDevice;

		bool Init();
		void Release();

		bool Load();
		void Unload();

		bool BeginFrame(vk::RenderContext& renderContext);
		void EndFrame();

		void Present(vk::Texture* texture);

		void CompleteAllCommands();

		bool RecreateSwapchain(uint32 screenWidth, uint32 screenHeight);
		bool RecreateSwapchain();

		auto CreateRenderPass()->vk::RenderPass*;
		void DestroyRenderPass(vk::RenderPass* renderPass);

		auto CreatePipeline()->vk::Pipeline*;
		void DestroyPipeline(vk::Pipeline* pipeline);

		auto CreateParameterSetDefinition(const df::StringView& name)->vk::ParameterSetDefinition*;
		void DestroyParameterSetDefinition(vk::ParameterSetDefinition* parameterSetDefinition);
		auto FindParameterSetDefinition(const df::StringView& name) const -> const vk::ParameterSetDefinition*;

		auto CreateParameterSet(const df::StringView& name)->vk::ParameterSet*;
		auto CreateParameterSet(vk::ParameterSetDefinition* parameterSetDefinition)->vk::ParameterSet*;
		void DestroyParameterSet(vk::ParameterSet* parameterSet);

		auto RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format, uint32 index) -> const vk::VertexAttribute*;
		auto FindVertexAttribute(const df::StringView& name) const -> const vk::VertexAttribute*;

		void AddShaderInclude(const df::StringView& name, const df::StringView& content);
		auto GetShaderInclude(const df::StringView& name) const -> const df::String&;

		auto GetShaderCompiler()->vk::ShaderCompiler*;

	public:
		void RemoveImage(VkImage image);
		void RemoveImageView(VkImageView imageView);
		void RemoveBuffer(VkBuffer buffer);
		void RemoveBufferView(VkBufferView bufferView);
		void RemoveDeviceMemory(VkDeviceMemory deviceMemory);
		void RemoveSampler(VkSampler sampler);
		void RemoveShaderModule(VkShaderModule shaderModule);
		void RemoveRenderPass(VkRenderPass renderPass);
		void RemoveDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSet descriptorSet);
		void RemoveDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout);
		void RemovePipelineLayout(VkPipelineLayout pipelineLayout);
		void RemoveFramebuffer(VkFramebuffer framebuffer);
		void RemovePipeline(VkPipeline pipeline);
		void RemoveDescriptorPool(VkDescriptorPool descriptorPool);
		void RemoveCommandPool(VkCommandPool commandPool);

	private:
		bool InitInstance();
		bool InitPhysicalDevice();
		bool InitDevice();

		bool CheckForPreventedCaptureSoft();

		bool InitCommandPools();
		void ReleaseCommandPools();

		bool InitDescriptorPools();
		void ReleaseDescriptorPools();

		bool InitFrameData(vk::FrameData& frameData);
		void ReleaseFrameData(vk::FrameData& frameData);

		bool InitCommandBuffers(vk::FrameData& frameData);
		void ReleaseCommandBuffers(vk::FrameData& frameData);

		void EmptyTrashCan();

	private:
		const df::Window& m_Window;

		uint32 m_NumFramesInFlight;
		uint32 m_FrameIndex;

		df::Vector<vk::FrameData> m_FrameData;

		vk::CommandBuffer m_TransferCommandBuffer;
		vk::ShaderCompiler* m_ShaderCompiler;

		df::ObjectPool<vk::RenderPass> m_RenderPasses;
		df::ObjectPool<vk::Pipeline> m_Pipelines;
		df::ObjectPool<vk::ParameterSetDefinition> m_ParameterSetDefinitions;
		df::ObjectPool<vk::ParameterSet> m_ParameterSets;
		df::ObjectPool<vk::VertexAttribute> m_VertexAttributes;

		df::HashMap<df::String, vk::ParameterSetDefinition*> m_ParameterSetDefinitionRegistry;
		df::HashMap<df::String, df::String> m_ShaderIncludes;
		df::String m_NullInclude;

		// Vk
		VkInstance m_VkInstance;
		VkPhysicalDevice m_VkPhysicalDevice;
		VkDevice m_VkDevice;

		uint32 m_PresentFamilyIndex;
		uint32 m_GraphicsFamilyIndex;
		uint32 m_TransferFamilyIndex;
		uint32 m_ComputeFamilyIndex;

		VkQueue m_PresentQueue;
		VkQueue m_GraphicsQueue;
		VkQueue m_TransferQueue;
		VkQueue m_ComputeQueue;

		df::Vector<VkCommandPool> m_GraphicsCommandPools;
		df::Vector<VkCommandPool> m_TransferCommandPools;

		vk::Presentation m_Presentation;
		vk::TransferBuffer m_TransferBuffer;

		VkDescriptorPool m_DescriptorPool;

		VkPhysicalDeviceProperties m_DeviceProperties;
		VkPhysicalDeviceFeatures m_DeviceFeatures;
		VkPhysicalDeviceLimits m_DeviceLimits;

		vk::ObjectManager m_VulkanObjectManager;
	};
}