#pragma once

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkCommandBuffer.h"
#endif

#include <DwarfRender/ObjectId.h>
#include <DwarfRender/Types.h>

namespace rf {
	class CommandBuffer {
	public:
		auto GetAPIData()->rf::api::CommandBuffer&;

		bool Begin();
		void End();

		void CopyBuffer(rf::BufferId srcBuffer, rf::BufferId dstBuffer, uint32 range);
		void CopyBufferToImage(rf::BufferId srcBuffer, rf::TextureId dstImage, uint32 width, uint32 height);

		void InitializeColorAttachment(rf::TextureId texture);
		void TransitionImageLayout(rf::TextureId texture, rf::EImageLayout oldLayout, rf::EImageLayout newLayout);

		void BeginRenderPass(rf::PassId renderPass, rf::FramebufferId framebuffer);
		void EndRenderPass();
		void SetLineWidth(float width);
		void BindPipeline(rf::PipelineId pipeline);
		void BindPipeline(rf::GraphicsPipelineId pipeline, uint32 instanceIdx);
		void BindDescriptorSet(rf::PipelineLayoutId pipelineLayout, rf::DescriptorSetId descriptorSet);
		void BindVertexBuffer(rf::BufferId buffer, uint32 binding, uint32 offset = 0);
		void BindIndexBuffer(rf::BufferId buffer);
		void Draw(uint32 vertexCount);
		void Draw(uint32 vertexOffset, uint32 vertexCount);
		void DrawIndexed(uint32 indexCount);
		void DrawInstanced(uint32 vertexCount, uint32 firstInstance, uint32 instanceCount);
		void DrawInstancedIndexed(uint32 indexCount, uint32 firstInstance, uint32 instanceCount);

		void SetScissor(uint32 x, uint32 y, uint32 width, uint32 height);

	private:
		api::CommandBuffer m_APIData;
	};
}