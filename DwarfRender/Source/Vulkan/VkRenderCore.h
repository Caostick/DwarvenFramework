#pragma once

#include "VkAPI.h"
#include "VkObjectManager.h"
#include "VkTransferBuffer.h"
#include "VkFrameData.h"
#include "VkPresentationPipeline.h"
#include "VkPresentation.h"
#include "VkVertexAttribute.h"
#include "VkSampler.h"
#include "VkBuffer.h"

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
	class GraphicsPipeline;
	class ParameterSetDefinition;
	class ParameterSet;
	class ShaderCompiler;
	class Buffer;
	class Mesh;
	class Texture;
}

namespace vk {
	class RenderCore {
	public:
		RenderCore();

		auto GetVkDevice() const->VkDevice;
		auto GetVkDescriptorPool() const->VkDescriptorPool;

		bool Init();
		void Release();

		void SetWindowSource(df::Window* window, vk::Texture* texture);

		auto BeginFrame()->vk::CommandBuffer*;
		void EndFrame();

		void CompleteAllCommands();

		auto CreateRenderPass()->vk::RenderPass*;
		void DestroyRenderPass(vk::RenderPass* renderPass);

		auto CreateGraphicsPipeline()->vk::GraphicsPipeline*;
		void DestroyGraphicsPipeline(vk::GraphicsPipeline* pipeline);

		auto CreateBuffer()->vk::Buffer*;
		void DestroyBuffer(vk::Buffer* buffer);

		auto CreateMesh()->vk::Mesh*;
		void DestroyMesh(vk::Mesh* mesh);

		auto CreateTexture()->vk::Texture*;
		void DestroyTexture(vk::Texture* texture);

		auto CreateParameterSetDefinition(const df::StringView& name)->vk::ParameterSetDefinition*;
		void DestroyParameterSetDefinition(vk::ParameterSetDefinition* parameterSetDefinition);
		auto FindParameterSetDefinition(const df::StringView& name) const -> const vk::ParameterSetDefinition*;

		auto CreateParameterSet(const df::StringView& className)->vk::ParameterSet*;
		auto CreateParameterSet(vk::ParameterSetDefinition* parameterSetDefinition)->vk::ParameterSet*;
		void DestroyParameterSet(vk::ParameterSet* parameterSet);

		auto RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format) -> const vk::VertexAttribute*;
		auto FindVertexAttribute(const df::StringView& name) const -> const vk::VertexAttribute*;

		void AddShaderInclude(const df::StringView& name, const df::StringView& content);
		auto GetShaderInclude(const df::StringView& name) const -> const df::String&;

		auto GetShaderCompiler()->vk::ShaderCompiler*;

		auto RequestSampler(const vk::SamplerState& state)->vk::Sampler*;

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
		void RemoveSwapchain(VkSwapchainKHR swapchain);
		void RemoveSemaphore(VkSemaphore semaphore);

		void SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset = 0);
		void SetImageData(VkImage image, const void* data, uint32 dataSize, uint32 width, uint32 height, int32 widthOffset = 0, int32 heightOffset = 0);

		void GenerateMips(vk::Texture* texture);

		auto GetBufferAlignment(const BufferUsageFlags& usage)const->uint32;

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
		uint32 m_NumFramesInFlight;
		uint32 m_FrameIndex;

		df::Vector<vk::FrameData> m_FrameData;

		vk::PresentationPipeline m_PresentationPipiline;
		vk::CommandBuffer m_TransferCommandBuffer;
		vk::ShaderCompiler* m_ShaderCompiler;

		df::ObjectPool<vk::RenderPass> m_RenderPasses;
		df::ObjectPool<vk::GraphicsPipeline> m_GraphicsPipelines;
		df::ObjectPool<vk::ParameterSetDefinition> m_ParameterSetDefinitions;
		df::ObjectPool<vk::ParameterSet> m_ParameterSets;
		df::ObjectPool<vk::VertexAttribute> m_VertexAttributes;
		df::ObjectPool<vk::Sampler> m_Samplers;
		df::ObjectPool<vk::Buffer> m_Buffers;
		df::ObjectPool<vk::Mesh> m_Meshes;
		df::ObjectPool<vk::Texture> m_Textures;

		df::ObjectPool<vk::Presentation> m_Presentations;
		df::Vector<vk::Presentation*> m_ValidPresentations;

		df::Vector<vk::Texture*> m_TexturesToInitLayout;
		df::Vector<vk::Texture*> m_TexturesToGenerateMips;

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

		vk::TransferBuffer m_TransferBuffer;

		VkDescriptorPool m_DescriptorPool;

		VkPhysicalDeviceProperties m_DeviceProperties;
		VkPhysicalDeviceFeatures m_DeviceFeatures;
		VkPhysicalDeviceLimits m_DeviceLimits;

		vk::ObjectManager m_VulkanObjectManager;
	};
}