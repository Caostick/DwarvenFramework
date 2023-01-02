#pragma once

#include <DwarfRender/CommandBuffer.h>

#include "VkScopedRenderEvent.h"

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace vk {
	class RenderPass;
	class GraphicsPipeline;
	class ParameterSet;
	class Texture;
	class Buffer;
}

namespace vk {
	enum class EImageLayout {
		Undefined,
		General,
		ColorAttachment,
		ColorReadOnly,
		DepthStencilAttachment,
		DepthStencilReadOnly,
		DepthReadOnlyStencilAttachment,
		DepthAttachmentStencilReadOnly,
		DepthAttachment,
		DepthReadOnly,
		StencilAttachment,
		StencilReadOnly,
		TransferSrc,
		TransferDst
	};
}

namespace vk {
	class CommandBuffer : public df::CommandBuffer {
	public:
		CommandBuffer();
		virtual ~CommandBuffer();

		virtual void BeginRenderPass(df::RenderPass* renderPass) override;
		virtual void EndRenderPass() override;

		virtual void BindPipeline(df::GraphicsPipeline* pipeline) override;
		virtual bool BindParameterSet(df::ParameterSet* parameterSet) override;

		virtual void Draw(uint32 vertexCount) override;
		virtual void Draw(df::Mesh* mesh, uint32 instanceCount = 1, uint32 instanceOffset = 0) override;

	public:
		auto Get() const->VkCommandBuffer;

		bool Init(const df::StringView& name, VkDevice device, VkCommandPool commandPool);
		void Release(VkDevice device, VkCommandPool commandPool);

		bool Begin();
		void End();

		void Submit(VkQueue queue);
		void Wait(VkQueue queue);

		void ImageLayoutTransition(VkImage image, uint32 mipCount, vk::EImageLayout oldLayout, vk::EImageLayout newLayout, int32 mip = -1, bool isDepth = false, bool isStencil = false);
		void ImageLayoutTransition(vk::Texture* texture, vk::EImageLayout oldLayout, vk::EImageLayout newLayout, int32 mip = -1);
		void CopyBuffer(VkBuffer src, VkBuffer dst, uint32 range, uint32 srcOffset = 0, uint32 dstOffset = 0);
		void CopyBufferToImage(VkBuffer src, VkImage dst, uint32 width, uint32 height, uint32 srcBufferOffset = 0, int32 widthOffset = 0, int32 heightOffset = 0);

		void GenerateMips(vk::Texture* texture);

		auto ScopedRenderEvent(const char* name)->vk::ScopedRenderEvent;

		void BeginRenderPass(vk::RenderPass* renderPass);

		void BindPipeline(vk::GraphicsPipeline* pipeline);
		bool BindParameterSet(vk::ParameterSet* parameterSet);

		void BindVertexBuffer(const vk::Buffer* buffer, uint32 binding, uint32 offset = 0);
		void BindIndexBuffer(const vk::Buffer* buffer);

		void ValidateState();

	private:
		VkCommandBuffer m_VkCommandBuffer;

		vk::RenderPass* m_CurrentRenderPass;
		vk::GraphicsPipeline* m_CurrentPipeline;
	};
}

#if DF_USE_DEBUG_MARKERS
#define DFScopedRenderEvent(rcb, name) const vk::ScopedRenderEvent _df_dbg_marker_ = rcb.ScopedRenderEvent(name)
#else
#define DFScopedRenderEvent(rcb, name)
#endif