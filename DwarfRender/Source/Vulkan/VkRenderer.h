#pragma once

#include <DwarfRender/Renderer.h>

#include "VkRenderCore.h"

namespace df {
	class Window;
}

namespace vk {
	class CommandBuffer;
}

namespace vk {
	class Renderer : public df::Renderer {
	public:
		Renderer();
		virtual ~Renderer() override;

		virtual bool Init() override;
		virtual void Release() override;

		virtual void SetWindowSource(df::Window* window, df::Texture* texture) override;

		virtual auto CreateMesh()->df::Mesh* override;
		virtual void DestroyMesh(df::Mesh* mesh) override;

		virtual auto CreateBuffer()->df::Buffer* override;
		virtual void DestroyBuffer(df::Buffer* buffer) override;

		virtual auto CreateTexture()->df::Texture* override;
		virtual void DestroyTexture(df::Texture* texture) override;

		virtual auto CreateRenderPass()->df::RenderPass* override;
		virtual void DestroyRenderPass(df::RenderPass* renderPass) override;

		virtual auto RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format) -> const df::VertexAttribute* override;
		virtual auto FindVertexAttribute(const df::StringView& name) const -> const df::VertexAttribute* override;

		virtual auto CreateParameterSet(const df::StringView& className)->df::ParameterSet* override;
		virtual void DestroyParameterSet(df::ParameterSet* parameterSet) override;

		virtual auto CreateGraphicsPipeline()->df::GraphicsPipeline* override;
		virtual void DestroyGraphicsPipeline(df::GraphicsPipeline* pipeline) override;

		virtual auto BeginFrame()->df::CommandBuffer* override;
		virtual void EndFrame() override;

		virtual auto GetSnippetProvider() const -> const df::SnippetProvider& override;

	private:
		vk::RenderCore m_RenderCore;
	};
}