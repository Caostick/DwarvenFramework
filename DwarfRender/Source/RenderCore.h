#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkRenderCore.h"
#endif

#include "CommandBuffer.h"

#include <DwarfRender/ObjectId.h>
#include <DwarfRender/VertexAttribute.h>

#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>


namespace df {
	class Window;
}

namespace rf {
	class RenderCore {
	public:
		RenderCore();
		~RenderCore();

		auto GetAPIData()->rf::api::RenderCore&;

		bool Init(df::Window* window);
		void Release();

		bool Load();
		void Unload();

		bool BeginFrame(rf::RenderContext& renderContext);
		void EndFrame();

		void Present(rf::TextureId texture);

		void CompleteAllCommands();

		bool RecreateSwapchain(uint32 screenWidth, uint32 screenHeight);
		bool RecreateSwapchain();

		auto GetNumFramesInFlight()const->uint32;
		auto GetBufferAlignment(const BufferUsageFlags& usage)const->uint32;

		auto RequestRasterizationState(const rf::RasterizationState& rasterizationState)->rf::RasterizationStateId;
		auto RequestBlendState(const rf::BlendState& blendState)->rf::BlendStateId;
		auto RequestDepthState(const rf::DepthState& depthState)->rf::DepthStateId;
		auto RequestEmptyVertexDescription()->rf::VertexDescriptionId;
		
		auto CreateSampler(const rf::SamplerState& samplerState)->rf::SamplerId;
		void DestroySampler(rf::SamplerId sampler);

		auto CreateBuffer(const df::StringView& name, uint32 size, rf::BufferUsageFlags usageFlags, rf::EBufferAccessType accessType, bool isStatic)->rf::BufferId;
		void SetBufferData(rf::BufferId buffer, const void* data, uint32 dataSize, uint32 offset = 0);
		void SyncBufferData(rf::BufferId buffer, uint32 frameIdx);
		void DestroyBuffer(rf::BufferId buffer);

		auto CreateTexture(const df::StringView& name, uint32 width, uint32 height, rf::ImageUsageFlags usage, rf::ETextureFormat format)->rf::TextureId;
		void SetTextureData(rf::TextureId texture, void* data, uint32 dataSize, uint32 offset = 0);
		void DestroyTexture(rf::TextureId texture);

		auto CreateMesh(uint32 vertexCount, uint32 indexCount)->MeshId;
		void SetMeshAttributeBuffer(MeshId mesh, uint32 attributeId, const void* data);
		void SetMeshIndexBuffer(MeshId mesh, const uint32* data);
		void DestroyMesh(MeshId mesh);
		
		auto CreateRenderPass(const df::StringView& name, const df::Vector<rf::AttachmentDefinition>& attachments)->rf::PassId;
		void DestroyRenderPass(rf::PassId renderPass);

		auto CreateDescriptorSetLayout(const df::StringView& name, const df::Vector<rf::DescriptorDefinition>& descriptors)->rf::DescriptorSetLayoutId;
		auto CreateDescriptorSetLayout(const df::StringView& name, const rf::DescriptorLayout& vsDescriptorLayout)->rf::DescriptorSetLayoutId;
		auto CreateDescriptorSetLayout(const df::StringView& name, const rf::DescriptorLayout& vsDescriptorLayout, const rf::DescriptorLayout& fsDescriptorLayout)->rf::DescriptorSetLayoutId;
		void DestroyDescriptorSetLayout(rf::DescriptorSetLayoutId descriptorSetLayout);

		auto CreateDescriptorSet(const df::StringView& name, uint32 id, rf::DescriptorSetLayoutId descriptorSetLayout)->rf::DescriptorSetId;
		void DestroyDescriptorSet(rf::DescriptorSetId descriptorSet);

		auto CreatePipelineLayout(const df::StringView& name, const df::Vector<rf::DescriptorSetLayoutId>& descriptorSets)->rf::PipelineLayoutId;
		void DestroyPipelineLayout(rf::PipelineLayoutId pipelineLayout);

		auto CreateFramebuffer(const df::StringView& name, rf::PassId renderPass, const df::Vector<rf::TextureId>& renderTargets)->rf::FramebufferId;
		void DestroyFramebuffer(rf::FramebufferId framebuffer);

		auto CreateShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ShaderModuleId;
		auto CreateShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ShaderModuleId;
		void DestroyShaderModule(rf::ShaderModuleId shaderModule);

		auto CreateVertexShaderModule(const df::StringView& name, const df::Vector<uint32>& code) -> rf::VertexShaderModuleId;
		auto CreateVertexShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::VertexShaderModuleId;
		void DestroyVertexShaderModule(rf::VertexShaderModuleId shaderModule);

		auto CreateFragmentShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::FragmentShaderModuleId;
		auto CreateFragmentShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::FragmentShaderModuleId;
		void DestroyFragmentShaderModule(rf::FragmentShaderModuleId shaderModule);

		auto CreateComputeShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ComputeShaderModuleId;
		auto CreateComputeShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ComputeShaderModuleId;
		void DestroyComputeShaderModule(rf::ComputeShaderModuleId shaderModule);

		auto CreatePipelineStateObject(const df::StringView& name, const rf::PipelineDescriptor& pipelineDescriptor)->rf::PipelineId;
		void DestroyPipelineStateObject(rf::PipelineId pipeline);

		auto CreateGraphicsPipeline(const df::StringView& name, rf::VertexShaderModuleId vertexShader, rf::FragmentShaderModuleId fragmentShader)->rf::GraphicsPipelineId;
		void DestroyGraphicsPipeline(rf::GraphicsPipelineId pipeline);

		auto CreateMaterialRule(const df::StringView& name)->rf::MaterialRuleId;
		void InitMaterialRule(rf::MaterialRuleId materialRule);
		void DestroyMaterialRule(rf::MaterialRuleId materialRule);

		auto CreateMaterial(rf::MaterialRuleId materialRule)->rf::MaterialId;
		void DestroyMaterial(rf::MaterialId material);

		auto RequestPipelineInstance(rf::GraphicsPipelineId pipeline, rf::PassId renderPass) -> uint32;
		auto InitPipelineInstance(rf::GraphicsPipelineId pipeline, rf::PassId renderPass) -> uint32;

	private:
		bool InitInstance();
		bool InitPhysicalDevice();
		bool InitDevice();

		bool CheckForPreventedCaptureSoft();

		bool InitCommandPools();
		void ReleaseCommandPools();

		bool InitDescriptorPools();
		void ReleaseDescriptorPools();

		bool InitFrameData(rf::FrameData& frameData);
		void ReleaseFrameData(rf::FrameData& frameData);

		bool InitCommandBuffers(rf::FrameData& frameData);
		void ReleaseCommandBuffers(rf::FrameData& frameData);

	private:
		rf::api::RenderCore m_APIData;

		rf::CommandBuffer m_TransferCommandBuffer;

		df::Vector<rf::FrameData> m_FrameData;

		uint32 m_NumFramesInFlight;
		uint32 m_FrameIndex;

		df::ObjectPool<rf::RasterizationState> m_RasterizationStates;
		df::ObjectPool<rf::BlendState> m_BlendStates;
		df::ObjectPool<rf::DepthState> m_DepthStates;
		df::ObjectPool<rf::VertexDescription> m_VertexDescriptions;
		df::ObjectPool<rf::Texture> m_Textures;
		df::ObjectPool<rf::Mesh> m_Meshes;
		df::ObjectPool<rf::ShaderModule> m_ShaderModules;
		df::ObjectPool<rf::VertexShaderModule> m_VertexShaderModules;
		df::ObjectPool<rf::FragmentShaderModule> m_FragmentShaderModules;
		df::ObjectPool<rf::ComputeShaderModule> m_ComputeShaderModules;
		df::ObjectPool<rf::Pass> m_RenderPasses;
		df::ObjectPool<rf::Framebuffer> m_Framebuffers;
		df::ObjectPool<rf::Buffer> m_Buffers;
		df::ObjectPool<rf::Sampler> m_Samplers;
		df::ObjectPool<rf::DescriptorSetLayout> m_DescriptorSetLayouts;
		df::ObjectPool<rf::DescriptorSet> m_DescriptorSets;
		df::ObjectPool<rf::PipelineLayout> m_PipelineLayouts;
		df::ObjectPool<rf::Pipeline> m_Pipelines;
		df::ObjectPool<rf::GraphicsPipeline> m_GraphicsPipelines;
		df::ObjectPool<rf::MaterialRule> m_MaterialRules;
		df::ObjectPool<rf::Material> m_Materials;
	};
}